//
// Created by revant-sinha on 4/28/26.
//

#ifndef MOVEHISTORY_H
#define MOVEHISTORY_H
#include <vector>
#include "../types_constants/types.h"

namespace MoveHistory {
    class MoveHistory {
        void addMoveToHistoryTable(Move &move);
    private:
        std::vector<U64> moveHistory;
    };
} // MoveHistory

#endif //MOVEHISTORY_H
