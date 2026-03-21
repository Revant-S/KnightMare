//
// Created by revant-sinha on 3/16/26.
//

#ifndef LEGALMOVEFILTER_H
#define LEGALMOVEFILTER_H
#include "../../board/Board.h"

namespace LegalMoveFilter {
    // std::vector<Move> filterIllegalMoves(Board &board, std::vector<Move> &moves);

    bool canKingCastle(Board &board, Move &move, Color color);

    bool isMoveLegal(Board &board, Move &move);
} // LegalMoveFilter

#endif //LEGALMOVEFILTER_H
