//
// Created by revant-sinha on 3/29/26.
//

#include "Search.h"

#include "../evaluation/Evaluation.h"
#include "../legal_move_generation/MoveFunctions.h"
#include <limits>
#include <algorithm>

namespace Search {
    int minMax(const int depth, Board &board) {
        if (depth == 0) {
            return Evaluation::evaluate(board);
        }

        int maxScore = std::numeric_limits<int>::min();
        MoveList legalMoves = MoveFunctions::getAllLegalMoves(board);

        if (legalMoves.isEmpty()) {
            if (MoveFunctions::isKingInCheck(board.getSide(), board)) {
                return -100000 + depth; // checkmate (depth helps prefer faster mates)
            } else {
                return 0; // stalemate
            }
        }
        for (Move &move: legalMoves) {
            BoardState savedState = board.saveState();
            board.makeMove(move);
            board.toggle_side();
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
        int maxScore = std::numeric_limits<int>::min();
        Move bestMove = legalMoves[0];
        for (auto &move: legalMoves) {
            BoardState savedState = board.saveState();
            board.makeMove(move);
            board.toggle_side();
            int score = -minMax(DEPTH_OF_SEARCH - 1, board);
            board.unmakeMove(savedState);
            if (score > maxScore) {
                maxScore = score;
                bestMove = move;
            }
        }
        return bestMove;
    }
} // Search
