//
// Created by revant-sinha on 3/29/26.
//

#ifndef SEARCH_H
#define SEARCH_H
#include "../../board/Board.h"
#include "../../types_constants/types.h"
#include "../../History/MoveHistory.h"

namespace Search {
    struct SearchLimits {
        int maxDepth = MAX_SEARCH_DEPTH;
        long long timeBudgetMs = -1;
    };

    long long computeTimeBudget(long long remainingMs, long long incrementMs, int movesToGo);

    Move getBestMove(Board &board, MoveHistory::MoveHistory &history, const SearchLimits &limits);

    int alphaBeta(Board &board, int depth, int alpha, int beta, int ply, bool allowNullMove);

    int quiescence(Board &board, int alpha, int beta, int ply);

    void orderMoves(MoveList &moves, std::array<int, 256> &scores, const Board &board, const Move &ttMove, int ply);
} // Search

#endif //SEARCH_H
