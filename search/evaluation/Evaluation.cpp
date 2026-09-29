//
// Created by revant-sinha on 3/29/26.
//

#include "Evaluation.h"
#include <bit>

#include "../../PreMatchComputations/PreMatchAttackComputation.h"
#include "../../utils/utils.h"
#include "../legal_move_generation/MoveFunctions.h"

namespace Evaluation {
    namespace {
        constexpr int TEMPO_BONUS = 10;
        constexpr int END_GAME_PHASE_THRESHOLD = 8;
        constexpr TaperedScore BISHOP_PAIR_BONUS = {30, 50};
        constexpr TaperedScore DOUBLED_PAWN_PENALTY = {-10, -20};
        constexpr TaperedScore ISOLATED_PAWN_PENALTY = {-12, -15};
        constexpr TaperedScore ROOK_OPEN_FILE_BONUS = {25, 10};
        constexpr TaperedScore ROOK_SEMI_OPEN_FILE_BONUS = {12, 5};
        constexpr TaperedScore ROOK_ON_SEVENTH_BONUS = {20, 10};
        constexpr int SHIELD_PAWN_CLOSE_BONUS = 15;
        constexpr int SHIELD_PAWN_FAR_BONUS = 8;
        constexpr int MISSING_SHIELD_PAWN_PENALTY = -15;
        constexpr int OPEN_FILE_NEAR_KING_PENALTY = -20;
        constexpr int MAX_KING_DANGER = 400;
        constexpr std::array<int, 6> kingAttackWeight = {0, 2, 2, 3, 5, 0};

        Color opponentOf(const Color side) {
            return side == WHITE ? BLACK : WHITE;
        }

        int relativeRank(const int square, const Color side) {
            const int rank = square / BOARD_WIDTH;
            return side == WHITE ? rank : 7 - rank;
        }

        int tableIndexFor(const int square, const Color side) {
            return side == WHITE ? square ^ 56 : square;
        }

        U64 fileMask(const int file) {
            return FILE_A_MASK << file;
        }

        U64 adjacentFilesMask(const int file) {
            U64 mask = 0;
            if (file > 0) mask |= fileMask(file - 1);
            if (file < 7) mask |= fileMask(file + 1);
            return mask;
        }

        U64 ranksAheadMask(const int square, const Color side) {
            const int rank = square / BOARD_WIDTH;
            if (side == WHITE) return rank == 7 ? 0 : ~0ULL << (BOARD_WIDTH * (rank + 1));
            return (1ULL << (BOARD_WIDTH * rank)) - 1;
        }

        U64 pawnAttackSpan(const Board &board, const Color side) {
            U64 attacks = 0;
            U64 pawns = board.getPieceBitBoard(PAWN, side);
            while (pawns) {
                const int square = Utils::getLSB(pawns);
                Utils::popLSB(pawns);
                attacks |= PreMatchAttackComputation::pawnAttacks[side][square];
            }
            return attacks;
        }

        U64 pieceAttacks(const Piece piece, const int square, const U64 occupancy) {
            switch (piece) {
                case KNIGHT: return PreMatchAttackComputation::knightAttacks[square];
                case BISHOP: return PreMatchAttackComputation::getBishopAttacks(square, occupancy);
                case ROOK: return PreMatchAttackComputation::getRookAttacks(square, occupancy);
                case QUEEN: return PreMatchAttackComputation::getRookAttacks(square, occupancy) |
                                   PreMatchAttackComputation::getBishopAttacks(square, occupancy);
                default: return 0;
            }
        }

        TaperedScore mobilityScore(const Piece piece, const int reachableSquares) {
            switch (piece) {
                case KNIGHT: return {4 * (reachableSquares - 4), 4 * (reachableSquares - 4)};
                case BISHOP: return {5 * (reachableSquares - 7), 5 * (reachableSquares - 7)};
                case ROOK: return {2 * (reachableSquares - 7), 4 * (reachableSquares - 7)};
                case QUEEN: return {reachableSquares - 14, 2 * (reachableSquares - 14)};
                default: return {};
            }
        }

        int centerDistance(const int square) {
            const int rank = square / BOARD_WIDTH;
            const int file = square % BOARD_WIDTH;
            return std::max(3 - rank, rank - 4) + std::max(3 - file, file - 4);
        }

        int manhattanDistance(const int first, const int second) {
            return std::abs(first / BOARD_WIDTH - second / BOARD_WIDTH) +
                   std::abs(first % BOARD_WIDTH - second % BOARD_WIDTH);
        }

        int nonPawnMaterial(const Board &board, const Color side) {
            int material = 0;
            for (int piece = KNIGHT; piece <= QUEEN; piece++) {
                material += materialWeight[piece] * std::popcount(board.getPieceBitBoard(static_cast<Piece>(piece), side));
            }
            return material;
        }
    }

    int gamePhase(const Board &board) {
        int phase = 0;
        for (int piece = KNIGHT; piece <= QUEEN; piece++) {
            const U64 bothSides = board.getPieceBitBoard(static_cast<Piece>(piece), WHITE) |
                                  board.getPieceBitBoard(static_cast<Piece>(piece), BLACK);
            phase += gamePhaseWeight[piece] * std::popcount(bothSides);
        }
        return std::min(phase, TOTAL_GAME_PHASE);
    }

    bool isEndGameReached(const Board &board) {
        return gamePhase(board) <= END_GAME_PHASE_THRESHOLD;
    }

    bool isInsufficientMaterial(const Board &board) {
        for (const Color side: {WHITE, BLACK}) {
            if (board.getPieceBitBoard(PAWN, side) | board.getPieceBitBoard(ROOK, side) |
                board.getPieceBitBoard(QUEEN, side))
                return false;
            if (std::popcount(board.getPieceBitBoard(KNIGHT, side) | board.getPieceBitBoard(BISHOP, side)) > 1)
                return false;
        }
        return true;
    }

    int blendByPhase(const TaperedScore &score, const int phase) {
        return (score.midGame * phase + score.endGame * (TOTAL_GAME_PHASE - phase)) / TOTAL_GAME_PHASE;
    }

    TaperedScore materialScore(const Board &board, const Color side) {
        TaperedScore score;
        for (int piece = PAWN; piece <= QUEEN; piece++) {
            const int count = std::popcount(board.getPieceBitBoard(static_cast<Piece>(piece), side));
            score += {materialWeight[piece] * count, endGameMaterialWeight[piece] * count};
        }
        if (std::popcount(board.getPieceBitBoard(BISHOP, side)) >= 2) score += BISHOP_PAIR_BONUS;
        return score;
    }

    TaperedScore pieceSquareScore(const Board &board, const Color side) {
        TaperedScore score;
        for (int piece = PAWN; piece <= KING; piece++) {
            const auto &midGameTable = pstTable[piece];
            const auto &endGameTable = piece == PAWN ? pawnEndGameTable
                                       : piece == KING ? pstTable[KING + 1]
                                       : pstTable[piece];
            U64 pieces = board.getPieceBitBoard(static_cast<Piece>(piece), side);
            while (pieces) {
                const int index = tableIndexFor(Utils::getLSB(pieces), side);
                Utils::popLSB(pieces);
                score += {midGameTable[index], endGameTable[index]};
            }
        }
        return score;
    }

    TaperedScore pawnStructureScore(const Board &board, const Color side) {
        TaperedScore score;
        const U64 ownPawns = board.getPieceBitBoard(PAWN, side);
        const U64 enemyPawns = board.getPieceBitBoard(PAWN, opponentOf(side));

        for (int file = 0; file < BOARD_WIDTH; file++) {
            const int pawnsOnFile = std::popcount(ownPawns & fileMask(file));
            if (pawnsOnFile == 0) continue;
            if (pawnsOnFile > 1) {
                score += {DOUBLED_PAWN_PENALTY.midGame * (pawnsOnFile - 1),
                          DOUBLED_PAWN_PENALTY.endGame * (pawnsOnFile - 1)};
            }
            if (!(ownPawns & adjacentFilesMask(file))) {
                score += {ISOLATED_PAWN_PENALTY.midGame * pawnsOnFile, ISOLATED_PAWN_PENALTY.endGame * pawnsOnFile};
            }
        }

        U64 pawns = ownPawns;
        while (pawns) {
            const int square = Utils::getLSB(pawns);
            Utils::popLSB(pawns);
            const int file = square % BOARD_WIDTH;
            const U64 frontSpan = (fileMask(file) | adjacentFilesMask(file)) & ranksAheadMask(square, side);
            if (!(frontSpan & enemyPawns)) {
                const int rank = relativeRank(square, side);
                score += {passedPawnMidGameBonus[rank], passedPawnEndGameBonus[rank]};
            }
        }
        return score;
    }

    TaperedScore pieceActivityScore(const Board &board, const Color side) {
        TaperedScore score;
        const U64 occupancy = board.getOccupancies(BOTH);
        const U64 safeSquares = ~board.getOccupancies(side) & ~pawnAttackSpan(board, opponentOf(side));
        const U64 ownPawns = board.getPieceBitBoard(PAWN, side);
        const U64 allPawns = ownPawns | board.getPieceBitBoard(PAWN, opponentOf(side));

        for (int piece = KNIGHT; piece <= QUEEN; piece++) {
            U64 pieces = board.getPieceBitBoard(static_cast<Piece>(piece), side);
            while (pieces) {
                const int square = Utils::getLSB(pieces);
                Utils::popLSB(pieces);
                const U64 reachable = pieceAttacks(static_cast<Piece>(piece), square, occupancy) & safeSquares;
                score += mobilityScore(static_cast<Piece>(piece), std::popcount(reachable));

                if (piece != ROOK) continue;
                const U64 rookFile = fileMask(square % BOARD_WIDTH);
                if (!(rookFile & allPawns)) score += ROOK_OPEN_FILE_BONUS;
                else if (!(rookFile & ownPawns)) score += ROOK_SEMI_OPEN_FILE_BONUS;
                if (relativeRank(square, side) == 6) score += ROOK_ON_SEVENTH_BONUS;
            }
        }
        return score;
    }

    TaperedScore kingSafetyScore(const Board &board, const Color side) {
        const U64 kingBitBoard = board.getPieceBitBoard(KING, side);
        if (!kingBitBoard) return {};
        const int kingSquare = Utils::getLSB(kingBitBoard);
        const Color enemy = opponentOf(side);
        int safety = 0;

        if (relativeRank(kingSquare, side) <= 1) {
            const U64 ownPawns = board.getPieceBitBoard(PAWN, side);
            const U64 allPawns = ownPawns | board.getPieceBitBoard(PAWN, enemy);
            const int forward = side == WHITE ? BOARD_WIDTH : -BOARD_WIDTH;
            const int kingFile = kingSquare % BOARD_WIDTH;
            for (int file = std::max(0, kingFile - 1); file <= std::min(7, kingFile + 1); file++) {
                const int shieldSquare = kingSquare - kingFile + file + forward;
                if (ownPawns & (1ULL << shieldSquare)) safety += SHIELD_PAWN_CLOSE_BONUS;
                else if (ownPawns & (1ULL << (shieldSquare + forward))) safety += SHIELD_PAWN_FAR_BONUS;
                else safety += MISSING_SHIELD_PAWN_PENALTY;
                if (!(allPawns & fileMask(file))) safety += OPEN_FILE_NEAR_KING_PENALTY;
            }
        }

        const U64 kingZone = PreMatchAttackComputation::kingAttacks[kingSquare] | kingBitBoard;
        const U64 occupancy = board.getOccupancies(BOTH);
        int attackerCount = 0;
        int attackWeight = 0;
        for (int piece = KNIGHT; piece <= QUEEN; piece++) {
            U64 attackers = board.getPieceBitBoard(static_cast<Piece>(piece), enemy);
            while (attackers) {
                const int square = Utils::getLSB(attackers);
                Utils::popLSB(attackers);
                if (pieceAttacks(static_cast<Piece>(piece), square, occupancy) & kingZone) {
                    attackerCount++;
                    attackWeight += kingAttackWeight[piece];
                }
            }
        }
        if (attackerCount >= 2) safety -= std::min(MAX_KING_DANGER, attackWeight * attackerCount * 4);

        return {safety, 0};
    }

    int mopUpScore(const Board &board) {
        const int materialBalance = nonPawnMaterial(board, WHITE) - nonPawnMaterial(board, BLACK);
        if (std::abs(materialBalance) < materialWeight[ROOK] - materialWeight[PAWN]) return 0;

        const Color strongSide = materialBalance > 0 ? WHITE : BLACK;
        const Color weakSide = opponentOf(strongSide);
        if (board.getPieceBitBoard(PAWN, weakSide) || !isEndGameReached(board)) return 0;

        const int strongKing = Utils::getLSB(board.getPieceBitBoard(KING, strongSide));
        const int weakKing = Utils::getLSB(board.getPieceBitBoard(KING, weakSide));
        const int bonus = 10 * centerDistance(weakKing) + 4 * (14 - manhattanDistance(strongKing, weakKing));
        return strongSide == WHITE ? bonus : -bonus;
    }

    int evaluate(Board &board) {
        if (isInsufficientMaterial(board)) return 0;

        TaperedScore whiteScore, blackScore;
        for (const Color side: {WHITE, BLACK}) {
            TaperedScore &score = side == WHITE ? whiteScore : blackScore;
            score += materialScore(board, side);
            score += pieceSquareScore(board, side);
            score += pawnStructureScore(board, side);
            score += pieceActivityScore(board, side);
            score += kingSafetyScore(board, side);
        }

        const int absoluteScore = blendByPhase(whiteScore - blackScore, gamePhase(board)) + mopUpScore(board);
        return (board.getSide() == WHITE ? absoluteScore : -absoluteScore) + TEMPO_BONUS;
    }
} // Evaluation
