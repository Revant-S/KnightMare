//
// Created by revant-sinha on 3/29/26.
//

#include "Evaluation.h"
#include <bit>


namespace Evaluation {

    // SIMPLEST POSSIBLE
    int evaluate(Board &board) {
        int whiteMaterial = 0;
        int blackMaterial = 0;

        for (int piece = PAWN; piece <= KING; piece++) {
            whiteMaterial += std::popcount(
                board.getPieceBitBoard(static_cast<Piece>(piece), WHITE)
            ) * materialWeight[piece];
            blackMaterial += std::popcount(
                board.getPieceBitBoard(static_cast<Piece>(piece), BLACK)
            ) * materialWeight[piece];
        }
        const int score = whiteMaterial - blackMaterial;
        return score * (board.getSide() == WHITE ? 1 : -1);
    }
} // Evaluation
