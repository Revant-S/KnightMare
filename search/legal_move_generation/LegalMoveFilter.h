//
// Created by revant-sinha on 3/16/26.
//

#ifndef LEGALMOVEFILTER_H
#define LEGALMOVEFILTER_H
#include "../../board/Board.h"

namespace LegalMoveFilter {
    void filterIllegalMoves(Board &board, std::vector<Move>& moves);
    bool isMoveLegal(Board &board, Move &move);
} // LegalMoveFilter

#endif //LEGALMOVEFILTER_H
