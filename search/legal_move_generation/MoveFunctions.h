//
// Created by revant-sinha on 3/17/26.
//

#ifndef MOVEFUNCTIONS_H
#define MOVEFUNCTIONS_H
#include "../../board/Board.h"
#include "../../types_constants/types.h"

namespace MoveFunctions {
    void makeMove(Move &move, Board &board);

    bool isKingInCheck(Color color, Board &board);
} // MoveFunctions

#endif //MOVEFUNCTIONS_H
