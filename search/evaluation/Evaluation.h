//
// Created by revant-sinha on 3/29/26.
//

#ifndef EVALUTATION_H
#define EVALUTATION_H
#include "../../board/Board.h"

namespace Evaluation {
    int evaluate(Board &board);

    bool isEndGameReached(Board &board);

    int compareMaterial(Board &board);

    int assesPieceDevelopment(Board &board);

    int totalMaterialValue(Board &board, Color side);

    int calculatePieceDevelopmentScore(Board &board, Color side, Piece piece);

    int materialValue(Board &board, Piece piece, Color side);

    std::array<int, 64> getPSTArray(Piece piece, Board &board);

    int kingSafetyScore(Board &board, Color side);
} // Evaluationa

#endif //EVALUTATION_H
