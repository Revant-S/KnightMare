//
// Created by revant-sinha on 3/29/26.
//

#include "Search.h"

#include "../evaluation/Evaluation.h"
#include "../legal_move_generation/MoveFunctions.h"
#include "../../TranspositionTable/TransPositionTable.h"
#include "../../utils/utils.h"
#include <chrono>
#include <iostream>

namespace Search {
    namespace {
        constexpr int INFINITY_SCORE = 1'000'000;
        constexpr int MATE_SCORE = 100'000;
        constexpr int MATE_THRESHOLD = MATE_SCORE - 1'000;
        constexpr int MAX_PLY = 128;
        constexpr long long TIME_CHECK_INTERVAL = 2048;
        constexpr int TT_MOVE_PRIORITY = 1'000'000;
        constexpr int CAPTURE_PRIORITY = 100'000;
        constexpr int PROMOTION_PRIORITY = 90'000;
        constexpr int FIRST_KILLER_PRIORITY = 80'000;
        constexpr int SECOND_KILLER_PRIORITY = 79'000;
        constexpr int MAX_HISTORY_PRIORITY = 70'000;
        constexpr int LOSING_CAPTURE_PRIORITY = -100'000;
        constexpr int DELTA_PRUNING_MARGIN = 200;
        constexpr int FIFTY_MOVE_RULE_PLIES = 100;
        constexpr int DEFAULT_MOVES_TO_GO = 20;
        constexpr int REVERSE_FUTILITY_MAX_DEPTH = 6;
        constexpr int REVERSE_FUTILITY_MARGIN_PER_DEPTH = 90;
        constexpr int NULL_MOVE_MIN_DEPTH = 3;
        constexpr int ASPIRATION_MIN_DEPTH = 5;
        constexpr int ASPIRATION_WINDOW = 40;
        constexpr long long MOVE_OVERHEAD_MS = 30;

        using Clock = std::chrono::steady_clock;
        Clock::time_point searchStart;
        long long timeBudgetMs = -1;
        long long nodesSearched = 0;
        bool stopRequested = false;
        MoveHistory::MoveHistory *gameHistory = nullptr;
        std::array<std::array<Move, 2>, MAX_PLY> killerMoves;
        std::array<std::array<std::array<int, 64>, 64>, 2> historyScores;

        bool isSameMove(const Move &first, const Move &second) {
            return first.from == second.from && first.to == second.to && first.promoteTo == second.promoteTo;
        }

        long long elapsedMs() {
            return std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - searchStart).count();
        }

        void checkTimeLimit() {
            if (nodesSearched % TIME_CHECK_INTERVAL == 0 && timeBudgetMs >= 0 && elapsedMs() >= timeBudgetMs)
                stopRequested = true;
        }

        int scoreToTable(const int score, const int ply) {
            if (score > MATE_THRESHOLD) return score + ply;
            if (score < -MATE_THRESHOLD) return score - ply;
            return score;
        }

        int scoreFromTable(const int score, const int ply) {
            if (score > MATE_THRESHOLD) return score - ply;
            if (score < -MATE_THRESHOLD) return score + ply;
            return score;
        }

        void makeSearchMove(Board &board, Move &move) {
            const bool isIrreversible = move.piece == PAWN || MoveFunctions::isCapture(move, board);
            board.makeMove(move);
            board.toggle_side();
            gameHistory->addPosition(board.getPositionHash(), isIrreversible);
        }

        void undoSearchMove(Board &board, const BoardState &savedState) {
            gameHistory->removeLastPosition();
            board.unmakeMove(savedState);
        }

        int moveOrderingScore(const Move &move, const Board &board, const Move &ttMove, const int ply) {
            if (isSameMove(move, ttMove)) return TT_MOVE_PRIORITY;
            if (MoveFunctions::isCapture(move, board)) {
                const Piece victim = move.moveType == EN_PASSANT ? PAWN : board.getPieceOnTheIndex(move.to).piece;
                const int mostValuableVictimFirst = 10 * victim - move.piece;
                const bool mayLoseMaterial = materialWeight[move.piece] > materialWeight[victim];
                if (mayLoseMaterial && MoveFunctions::staticExchange(move, board) < 0)
                    return LOSING_CAPTURE_PRIORITY + mostValuableVictimFirst;
                return CAPTURE_PRIORITY + mostValuableVictimFirst;
            }
            if (move.moveType == PROMOTION) return PROMOTION_PRIORITY + move.promoteTo;
            if (ply < MAX_PLY) {
                if (isSameMove(move, killerMoves[ply][0])) return FIRST_KILLER_PRIORITY;
                if (isSameMove(move, killerMoves[ply][1])) return SECOND_KILLER_PRIORITY;
            }
            return std::min(historyScores[move.colorOfPieceToMove][move.from][move.to], MAX_HISTORY_PRIORITY);
        }

        void pickNextMove(MoveList &moves, std::array<int, 256> &scores, const int startIndex) {
            int bestIndex = startIndex;
            for (int index = startIndex + 1; index < moves.size(); index++) {
                if (scores[index] > scores[bestIndex]) bestIndex = index;
            }
            std::swap(moves[startIndex], moves[bestIndex]);
            std::swap(scores[startIndex], scores[bestIndex]);
        }

        void recordQuietCutoff(const Move &move, const int depth, const int ply) {
            if (ply < MAX_PLY && !isSameMove(move, killerMoves[ply][0])) {
                killerMoves[ply][1] = killerMoves[ply][0];
                killerMoves[ply][0] = move;
            }
            historyScores[move.colorOfPieceToMove][move.from][move.to] += depth * depth;
        }

        bool hasNonPawnMaterial(const Board &board, const Color side) {
            return board.getPieceBitBoard(KNIGHT, side) | board.getPieceBitBoard(BISHOP, side) |
                   board.getPieceBitBoard(ROOK, side) | board.getPieceBitBoard(QUEEN, side);
        }

        int lateMoveReduction(const int depth, const int moveIndex, const bool isQuiet, const bool inCheck) {
            if (depth < 3 || moveIndex < 4 || !isQuiet || inCheck) return 0;
            return moveIndex >= 12 ? 2 : 1;
        }

        std::string formatScore(const int score) {
            if (score > MATE_THRESHOLD) return "mate " + std::to_string((MATE_SCORE - score + 1) / 2);
            if (score < -MATE_THRESHOLD) return "mate -" + std::to_string((MATE_SCORE + score) / 2);
            return "cp " + std::to_string(score);
        }

        std::string principalVariation(Board board, const int depth) {
            std::string line;
            for (int ply = 0; ply < depth; ply++) {
                TranspositionTable::Entry entry;
                if (!TranspositionTable::probe(board.getPositionHash(), entry) || entry.bestMove.from == -1) break;
                bool isLegal = false;
                for (Move &move: MoveFunctions::getAllLegalMoves(board)) {
                    if (isSameMove(move, entry.bestMove)) isLegal = true;
                }
                if (!isLegal) break;
                line += Utils::moveToString(entry.bestMove) + " ";
                board.makeMove(entry.bestMove);
                board.toggle_side();
            }
            return line;
        }

        void resetSearchState(MoveHistory::MoveHistory &history, const SearchLimits &limits) {
            searchStart = Clock::now();
            timeBudgetMs = limits.timeBudgetMs;
            nodesSearched = 0;
            stopRequested = false;
            gameHistory = &history;
            for (auto &killers: killerMoves) killers = {Move{}, Move{}};
            for (auto &colorScores: historyScores)
                for (auto &fromScores: colorScores) fromScores.fill(0);
        }
    }

    long long computeTimeBudget(const long long remainingMs, const long long incrementMs, const int movesToGo) {
        const int movesLeft = movesToGo > 0 ? movesToGo : DEFAULT_MOVES_TO_GO;
        const long long budget = std::min(remainingMs / movesLeft + incrementMs * 3 / 4, remainingMs / 3);
        return std::max(budget - MOVE_OVERHEAD_MS, 5LL);
    }

    void orderMoves(MoveList &moves, std::array<int, 256> &scores, const Board &board, const Move &ttMove,
                    const int ply) {
        for (int index = 0; index < moves.size(); index++) {
            scores[index] = moveOrderingScore(moves[index], board, ttMove, ply);
        }
    }

    int quiescence(Board &board, int alpha, const int beta, const int ply) {
        nodesSearched++;
        checkTimeLimit();
        if (stopRequested) return 0;
        if (ply >= MAX_PLY - 1) return Evaluation::evaluate(board);

        const bool inCheck = MoveFunctions::isKingInCheck(board.getSide(), board);
        int bestScore = -INFINITY_SCORE;
        int standPat = -INFINITY_SCORE;
        if (!inCheck) {
            standPat = Evaluation::evaluate(board);
            if (standPat >= beta) return standPat;
            alpha = std::max(alpha, standPat);
            bestScore = standPat;
        }

        MoveList moves = inCheck ? MoveFunctions::getAllLegalMoves(board) : MoveFunctions::getAllLegalCaptures(board);
        if (inCheck && moves.isEmpty()) return -MATE_SCORE + ply;
        std::array<int, 256> scores{};
        orderMoves(moves, scores, board, Move{}, ply);

        for (int index = 0; index < moves.size(); index++) {
            pickNextMove(moves, scores, index);
            Move &move = moves[index];
            if (!inCheck && move.moveType != PROMOTION) {
                const Piece victim = move.moveType == EN_PASSANT ? PAWN : board.getPieceOnTheIndex(move.to).piece;
                if (standPat + materialWeight[victim] + DELTA_PRUNING_MARGIN < alpha) continue;
                if (scores[index] < 0) continue;
            }

            const BoardState savedState = board.saveState();
            board.makeMove(move);
            board.toggle_side();
            const int score = -quiescence(board, -beta, -alpha, ply + 1);
            board.unmakeMove(savedState);
            if (stopRequested) return 0;

            if (score > bestScore) bestScore = score;
            if (score >= beta) return score;
            alpha = std::max(alpha, score);
        }
        return bestScore;
    }

    int alphaBeta(Board &board, int depth, int alpha, const int beta, const int ply, const bool allowNullMove) {
        const U64 positionHash = board.getPositionHash();
        if (ply > 0 && (gameHistory->isRepetition(positionHash) ||
                        gameHistory->pliesSinceIrreversibleMove() >= FIFTY_MOVE_RULE_PLIES))
            return 0;

        const bool inCheck = MoveFunctions::isKingInCheck(board.getSide(), board);
        if (inCheck) depth++;
        if (depth <= 0) return quiescence(board, alpha, beta, ply);
        if (ply >= MAX_PLY - 1) return Evaluation::evaluate(board);

        nodesSearched++;
        checkTimeLimit();
        if (stopRequested) return 0;

        const bool isPvNode = beta - alpha > 1;
        Move ttMove;
        TranspositionTable::Entry entry;
        if (TranspositionTable::probe(positionHash, entry)) {
            ttMove = entry.bestMove;
            if (!isPvNode && entry.depth >= depth) {
                const int ttScore = scoreFromTable(entry.score, ply);
                if (entry.bound == TranspositionTable::EXACT) return ttScore;
                if (entry.bound == TranspositionTable::LOWER_BOUND && ttScore >= beta) return ttScore;
                if (entry.bound == TranspositionTable::UPPER_BOUND && ttScore <= alpha) return ttScore;
            }
        }

        if (!isPvNode && !inCheck) {
            const int staticEval = Evaluation::evaluate(board);
            if (depth <= REVERSE_FUTILITY_MAX_DEPTH && std::abs(beta) < MATE_THRESHOLD &&
                staticEval - REVERSE_FUTILITY_MARGIN_PER_DEPTH * depth >= beta)
                return staticEval;

            if (allowNullMove && depth >= NULL_MOVE_MIN_DEPTH && staticEval >= beta &&
                hasNonPawnMaterial(board, board.getSide())) {
                const int reduction = 3 + depth / 6;
                const BoardState savedState = board.saveState();
                board.makeNullMove();
                gameHistory->addPosition(board.getPositionHash(), true);
                const int score = -alphaBeta(board, depth - 1 - reduction, -beta, -beta + 1, ply + 1, false);
                undoSearchMove(board, savedState);
                if (stopRequested) return 0;
                if (score >= beta) return score > MATE_THRESHOLD ? beta : score;
            }
        }

        MoveList moves = MoveFunctions::getAllLegalMoves(board);
        if (moves.isEmpty()) return inCheck ? -MATE_SCORE + ply : 0;

        std::array<int, 256> scores{};
        orderMoves(moves, scores, board, ttMove, ply);

        const int originalAlpha = alpha;
        int bestScore = -INFINITY_SCORE;
        Move bestMove;
        for (int index = 0; index < moves.size(); index++) {
            pickNextMove(moves, scores, index);
            Move &move = moves[index];
            const bool isQuiet = !MoveFunctions::isCapture(move, board) && move.moveType != PROMOTION;

            const BoardState savedState = board.saveState();
            makeSearchMove(board, move);
            int score;
            if (index == 0) {
                score = -alphaBeta(board, depth - 1, -beta, -alpha, ply + 1, true);
            } else {
                const int reduction = lateMoveReduction(depth, index, isQuiet, inCheck);
                score = -alphaBeta(board, depth - 1 - reduction, -alpha - 1, -alpha, ply + 1, true);
                if (score > alpha && reduction > 0)
                    score = -alphaBeta(board, depth - 1, -alpha - 1, -alpha, ply + 1, true);
                if (score > alpha && score < beta)
                    score = -alphaBeta(board, depth - 1, -beta, -alpha, ply + 1, true);
            }
            undoSearchMove(board, savedState);
            if (stopRequested) return 0;

            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
            if (score > alpha) alpha = score;
            if (alpha >= beta) {
                if (isQuiet) recordQuietCutoff(move, depth, ply);
                break;
            }
        }

        const TranspositionTable::Bound bound = bestScore <= originalAlpha
                                                    ? TranspositionTable::UPPER_BOUND
                                                    : bestScore >= beta
                                                          ? TranspositionTable::LOWER_BOUND
                                                          : TranspositionTable::EXACT;
        TranspositionTable::store(positionHash, depth, scoreToTable(bestScore, ply), bound,
                                  bound == TranspositionTable::UPPER_BOUND ? Move{} : bestMove);
        return bestScore;
    }

    namespace {
        struct RootResult {
            Move bestMove;
            int bestScore = -INFINITY_SCORE;
        };

        RootResult searchRoot(Board &board, MoveList &rootMoves, const Move &previousBest, const int depth,
                              int alpha, const int beta) {
            std::array<int, 256> scores{};
            orderMoves(rootMoves, scores, board, previousBest, 0);
            RootResult result;
            for (int index = 0; index < rootMoves.size(); index++) {
                pickNextMove(rootMoves, scores, index);
                Move &move = rootMoves[index];

                const BoardState savedState = board.saveState();
                makeSearchMove(board, move);
                int score;
                if (index == 0) {
                    score = -alphaBeta(board, depth - 1, -beta, -alpha, 1, true);
                } else {
                    score = -alphaBeta(board, depth - 1, -alpha - 1, -alpha, 1, true);
                    if (score > alpha && score < beta && !stopRequested)
                        score = -alphaBeta(board, depth - 1, -beta, -alpha, 1, true);
                }
                undoSearchMove(board, savedState);
                if (stopRequested) break;

                if (score > result.bestScore) {
                    result.bestScore = score;
                    result.bestMove = move;
                }
                if (score > alpha) alpha = score;
                if (alpha >= beta) break;
            }
            return result;
        }
    }

    Move getBestMove(Board &board, MoveHistory::MoveHistory &history, const SearchLimits &limits) {
        resetSearchState(history, limits);
        MoveList rootMoves = MoveFunctions::getAllLegalMoves(board);
        if (rootMoves.isEmpty()) return {};
        if (rootMoves.size() == 1) return rootMoves[0];

        Move bestMove = rootMoves[0];
        int previousScore = 0;
        for (int depth = 1; depth <= limits.maxDepth; depth++) {
            int windowAlpha = -INFINITY_SCORE, windowBeta = INFINITY_SCORE;
            if (depth >= ASPIRATION_MIN_DEPTH && std::abs(previousScore) < MATE_THRESHOLD) {
                windowAlpha = previousScore - ASPIRATION_WINDOW;
                windowBeta = previousScore + ASPIRATION_WINDOW;
            }

            RootResult result;
            while (true) {
                result = searchRoot(board, rootMoves, bestMove, depth, windowAlpha, windowBeta);
                if (stopRequested) break;
                if (result.bestScore <= windowAlpha) windowAlpha = -INFINITY_SCORE;
                else if (result.bestScore >= windowBeta) windowBeta = INFINITY_SCORE;
                else break;
            }

            if (result.bestMove.from != -1 && (!stopRequested || result.bestScore > windowAlpha))
                bestMove = result.bestMove;
            if (stopRequested) break;
            previousScore = result.bestScore;

            TranspositionTable::store(board.getPositionHash(), depth, result.bestScore, TranspositionTable::EXACT,
                                      bestMove);
            const long long elapsed = elapsedMs();
            std::cout << "info depth " << depth
                    << " score " << formatScore(result.bestScore)
                    << " nodes " << nodesSearched
                    << " time " << elapsed
                    << " nps " << nodesSearched * 1000 / std::max(elapsed, 1LL)
                    << " pv " << principalVariation(board, depth) << "\n";
            std::cout.flush();

            if (std::abs(result.bestScore) > MATE_THRESHOLD && depth > 1) break;
            if (timeBudgetMs >= 0 && elapsed * 2 >= timeBudgetMs) break;
        }
        return bestMove;
    }
} // Search
