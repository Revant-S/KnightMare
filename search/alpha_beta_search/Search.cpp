//
// Created by revant-sinha on 3/29/26.
//

#include "Search.h"

#include "../evaluation/Evaluation.h"
#include "../legal_move_generation/MoveFunctions.h"
#include <limits>
#include <algorithm>

#include "../../utils/utils.h"
#include "../evaluation/EvaluationUtils.h"

namespace Search {
    // Safe infinity values to prevent Negamax overflow
    const int INF = 1000000;
    const int MATE_SCORE = -100000;

    int minMax(const int depth, Board &board) {
        // Base case: static evaluation
        if (depth == 0) {
            return Evaluation::evaluate(board);
        }

        int maxScore = -INF;
        MoveList legalMoves = MoveFunctions::getAllLegalMoves(board);

        // Checkmate and Stalemate =
        if (legalMoves.isEmpty()) {
            if (MoveFunctions::isKingInCheck(board.getSide(), board)) {
                return MATE_SCORE - depth;
            } else {
                return 0; // stalemate
            }
        }

        for (Move &move: legalMoves) {
            BoardState savedState = board.saveState();
            board.makeMove(move);
            int score = -minMax(depth - 1, board);
            board.unmakeMove(savedState);
            maxScore = std::max(maxScore, score);
        }
        return maxScore;
    }

    Move getBestMove(Board &board) {
        MoveList legalMoves = MoveFunctions::getAllLegalMoves(board);
        if (legalMoves.isEmpty()) {
            return {};
        }

        int maxScore = -INF;
        Move bestMove = legalMoves[0];

        // Ensure DEPTH_OF_SEARCH is at least 2, otherwise it can't see mates!
        int currentDepth = std::max(2, DEPTH_OF_SEARCH);
        for (auto &move: legalMoves) {
            BoardState savedState = board.saveState();
            board.makeMove(move);
            // board.toggle_side(); // REMOVE THIS IF makeMove() ALREADY TOGGLES THE TURN
            int score = -minMax(currentDepth - 1, board);
            // EvaluationUtils::printScoreBreakDown(board, move);
            board.unmakeMove(savedState);
            // std::cout << "Move : " << Utils::moveToString(move) << " has calculate score of <<" << score << "\n";
            if (score > maxScore) {
                maxScore = score;
                bestMove = move;
            }
        }
        return bestMove;
    }

    MoveList OrderMoves(MoveList &moveList) {
    }
} // Search
