//
// Created by revant-sinha on 3/17/26.
//

#ifndef MOVEFUNCTIONS_H
#define MOVEFUNCTIONS_H
#include "../../board/Board.h"
#include "../../types_constants/types.h"

namespace MoveFunctions {
    bool isKingInCheck(Color color, Board &board);

    bool isSquareAttackedByEnemy(Color color, int squareIndex, Board &board);

    MoveList getAllLegalMoves(Board &board);

    MoveList getAllLegalCaptures(Board &board);

    bool isCapture(const Move &move, const Board &board);

    U64 getRookAttacks(int square, U64 occupancy);

    U64 getBishopAttacks(int square, U64 occupancy);
} // MoveFunctions

#endif //MOVEFUNCTIONS_H
