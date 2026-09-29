//
// Created by revant-sinha on 3/17/26.
//

#include "MoveFunctions.h"

#include "GeneratePseudoLegalMove.h"
#include "LegalMoveFilter.h"
#include "../../PreMatchComputations/PreMatchAttackComputation.h"
#include "../../utils/utils.h"

namespace MoveFunctions {
    namespace {
        struct KingSafety {
            int kingSquare;
            U64 checkers;
            U64 pinned;
        };

        KingSafety analyseKingSafety(const Board &board) {
            const Color side = board.getSide();
            const Color enemy = side == WHITE ? BLACK : WHITE;
            const int kingSquare = Utils::getLSB(board.getPieceBitBoard(KING, side));
            const U64 occupancy = board.getOccupancies(BOTH);
            const U64 enemyQueens = board.getPieceBitBoard(QUEEN, enemy);

            U64 snipers = (PreMatchAttackComputation::getRookAttacks(kingSquare, 0) &
                           (board.getPieceBitBoard(ROOK, enemy) | enemyQueens)) |
                          (PreMatchAttackComputation::getBishopAttacks(kingSquare, 0) &
                           (board.getPieceBitBoard(BISHOP, enemy) | enemyQueens));
            U64 pinned = 0;
            while (snipers) {
                const int sniper = Utils::getLSB(snipers);
                Utils::popLSB(snipers);
                const U64 blockers = PreMatchAttackComputation::squaresBetween[kingSquare][sniper] & occupancy;
                if (blockers && !(blockers & (blockers - 1))) pinned |= blockers & board.getOccupancies(side);
            }
            return {kingSquare, attackersTo(kingSquare, enemy, occupancy, board), pinned};
        }

        bool isLegal(Board &board, Move &move, const KingSafety &safety) {
            const U64 toBit = 1ULL << move.to;
            if (move.piece == KING) {
                if (move.moveType == CASTLE_KING_SIDE || move.moveType == CASTLE_QUEEN_SIDE)
                    return LegalMoveFilter::canKingCastle(board, move, board.getSide());
                const Color enemy = board.getSide() == WHITE ? BLACK : WHITE;
                const U64 occupancyWithoutKing = board.getOccupancies(BOTH) ^ (1ULL << move.from);
                return !attackersTo(move.to, enemy, occupancyWithoutKing, board);
            }
            if (move.moveType == EN_PASSANT) return LegalMoveFilter::isMoveLegal(board, move);
            if (safety.checkers) {
                if (safety.checkers & (safety.checkers - 1)) return false;
                const int checker = Utils::getLSB(safety.checkers);
                if (!(toBit & (safety.checkers | PreMatchAttackComputation::squaresBetween[safety.kingSquare][checker])))
                    return false;
            }
            if (safety.pinned & (1ULL << move.from))
                return PreMatchAttackComputation::lineThrough[safety.kingSquare][move.from] & toBit;
            return true;
        }

        void generatePseudoLegalMoves(Board &board, MoveList &moves) {
            GeneratePseudoLegalMove::getPawnPseudoLegalMoves(board, moves);
            GeneratePseudoLegalMove::getKnightPseudoLegalMoves(board, moves);
            GeneratePseudoLegalMove::getBishopPseudoLegalMoves(board, moves);
            GeneratePseudoLegalMove::getRookPseudoLegalMoves(board, moves);
            GeneratePseudoLegalMove::getQueenPseudoLegalMoves(board, moves);
            GeneratePseudoLegalMove::getKingPseudoLegalMoves(board, moves);
        }

        template<typename Filter>
        MoveList collectLegalMoves(Board &board, Filter keepMove) {
            MoveList pseudoMoves;
            generatePseudoLegalMoves(board, pseudoMoves);
            const KingSafety safety = analyseKingSafety(board);
            MoveList legalMoves;
            for (Move &move: pseudoMoves) {
                if (keepMove(move) && isLegal(board, move, safety)) legalMoves.addMove(move);
            }
            return legalMoves;
        }
    }

    U64 attackersTo(const int square, const Color attacker, const U64 occupancy, const Board &board) {
        const Color defender = attacker == WHITE ? BLACK : WHITE;
        const U64 queens = board.getPieceBitBoard(QUEEN, attacker);
        return (PreMatchAttackComputation::knightAttacks[square] & board.getPieceBitBoard(KNIGHT, attacker)) |
               (PreMatchAttackComputation::pawnAttacks[defender][square] & board.getPieceBitBoard(PAWN, attacker)) |
               (PreMatchAttackComputation::kingAttacks[square] & board.getPieceBitBoard(KING, attacker)) |
               (PreMatchAttackComputation::getRookAttacks(square, occupancy) &
                (board.getPieceBitBoard(ROOK, attacker) | queens)) |
               (PreMatchAttackComputation::getBishopAttacks(square, occupancy) &
                (board.getPieceBitBoard(BISHOP, attacker) | queens));
    }

    bool isKingInCheck(Color color, Board &board) {
        const U64 kingBitBoard = board.getPieceBitBoard(KING, color);
        const int kingIndex = Utils::getLSB(kingBitBoard);
        return isSquareAttackedByEnemy(color, kingIndex, board);
    }

    bool isSquareAttackedByEnemy(Color color, int squareIndex, Board &board) {
        const Color enemy = (color == WHITE) ? BLACK : WHITE;
        return attackersTo(squareIndex, enemy, board.getOccupancies(BOTH), board);
    }

    MoveList getAllLegalMoves(Board &board) {
        return collectLegalMoves(board, [](const Move &) { return true; });
    }

    MoveList getAllLegalCaptures(Board &board) {
        return collectLegalMoves(board, [&](const Move &move) {
            return isCapture(move, board) || (move.moveType == PROMOTION && move.promoteTo == QUEEN);
        });
    }

    bool isCapture(const Move &move, const Board &board) {
        if (move.moveType == EN_PASSANT) return true;
        const Color enemy = (move.colorOfPieceToMove == WHITE) ? BLACK : WHITE;
        return board.getOccupancies(enemy) & (1ULL << move.to);
    }

    int staticExchange(const Move &move, const Board &board) {
        std::array<int, 32> gain{};
        U64 occupancy = board.getOccupancies(BOTH) ^ (1ULL << move.from);
        if (move.moveType == EN_PASSANT) {
            occupancy ^= 1ULL << (move.colorOfPieceToMove == WHITE ? move.to - BOARD_WIDTH : move.to + BOARD_WIDTH);
            gain[0] = materialWeight[PAWN];
        } else {
            gain[0] = isCapture(move, board) ? materialWeight[board.getPieceOnTheIndex(move.to).piece] : 0;
        }

        Piece pieceOnTarget = move.moveType == PROMOTION ? move.promoteTo : move.piece;
        Color side = move.colorOfPieceToMove;
        int exchangeDepth = 0;
        while (exchangeDepth < 31) {
            side = side == WHITE ? BLACK : WHITE;
            const U64 attackers = attackersTo(move.to, side, occupancy, board) & occupancy;
            if (!attackers) break;

            exchangeDepth++;
            gain[exchangeDepth] = materialWeight[pieceOnTarget] - gain[exchangeDepth - 1];

            for (int piece = PAWN; piece <= KING; piece++) {
                const U64 candidates = attackers & board.getPieceBitBoard(static_cast<Piece>(piece), side);
                if (!candidates) continue;
                occupancy ^= candidates & -candidates;
                pieceOnTarget = static_cast<Piece>(piece);
                break;
            }
        }
        while (exchangeDepth > 0) {
            gain[exchangeDepth - 1] = -std::max(-gain[exchangeDepth - 1], gain[exchangeDepth]);
            exchangeDepth--;
        }
        return gain[0];
    }
} // MoveFunctions
