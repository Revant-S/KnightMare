//
// Created by revant-sinha on 3/29/26.
//

#ifndef SEARCH_H
#define SEARCH_H
#include "../../board/Board.h"
#include "../../types_constants/types.h"

namespace Search {
    int minMax(int depth , Board &board);
    int alphaBeta();
    Move getBestMove(Board &board);
} // Search

#endif //SEARCH_H
