//
// Created by revant-sinha on 3/29/26.
//

#include "Evaluation.h"
#include <bit>

namespace Evaluation {
    // SIMPLEST POSSIBLE
    int evaluate(Board &board) {
        int score = 0;
        for (int piece = PAWN; piece <= KING; piece++) {
            U64 whiteBB = board.getPieceBitBoard(static_cast<Piece>(piece), WHITE);
            U64 blackBB = board.getPieceBitBoard(static_cast<Piece>(piece), BLACK);

            while (whiteBB) {
                int sq = std::countr_zero(whiteBB);
                whiteBB &= whiteBB - 1;

                score += materialWeight[piece];

                if (piece == PAWN) score += pawnTable[sq];
                if (piece == KNIGHT) score += knightTable[sq];
            }

            while (blackBB) {
                int sq = std::countr_zero(blackBB);
                blackBB &= blackBB - 1;

                score -= materialWeight[piece];

                // mirror square for black
                int mirroredSq = sq ^ 56;

                if (piece == PAWN) score -= pawnTable[mirroredSq];
                if (piece == KNIGHT) score -= knightTable[mirroredSq];
            }
        }

        return (board.getSide() == WHITE ? score : -score);
    }
} // Evaluation
