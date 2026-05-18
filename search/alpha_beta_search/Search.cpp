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

    int minMax(const int depth, int alpha, int beta, Board &board) { //alpha, beta parameters added
        // Base case: static evaluation
        if (depth == 0) {
            return Evaluation::evaluate(board);
        }

        //int maxScore = -INF; removed this, alpha tracks
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
            int score = -minMax(depth - 1, -beta, -alpha, board); //- sign, because we basically reverse the limits
            //when the model predicts future opponent moves(imp: Swap alpha and beta position)
            board.unmakeMove(savedState);
            //maxScore = std::max(maxScore, score);
            if (score >= beta) return beta;
            if (score > alpha) alpha =  score;
        }
        return alpha;
    }

    Move getBestMove(Board &board) {
        MoveList legalMoves = MoveFunctions::getAllLegalMoves(board);
        if (legalMoves.isEmpty()) {
            return {};
        }

        //int maxScore = -INF;
        Move bestMove = legalMoves[0];
        int alpha = -INF;
        int beta = INF;

        // Ensure DEPTH_OF_SEARCH is at least 2, otherwise it can't see mates!
        int currentDepth = std::max(2, DEPTH_OF_SEARCH);
        for (auto &move: legalMoves) {
            BoardState savedState = board.saveState();
            board.makeMove(move);
            // board.toggle_side(); // REMOVE THIS IF makeMove() ALREADY TOGGLES THE TURN
            int score = -minMax(currentDepth - 1, -beta, -alpha, board);
            // EvaluationUtils::printScoreBreakDown(board, move);
            board.unmakeMove(savedState);
            // std::cout << "Move : " << Utils::moveToString(move) << " has calculate score of <<" << score << "\n";
            if (score > alpha) {
                alpha = score;
                bestMove = move;
            }
        }
        return bestMove;
    }

    MoveList OrderMoves(MoveList &moveList) {
        return moveList; //need to change, just to pass compilation, returning same
    }
} // Search
