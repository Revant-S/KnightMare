//
// Created by revant-sinha on 4/28/26.
//

#ifndef MOVEHISTORY_H
#define MOVEHISTORY_H
#include <vector>
#include "../types_constants/types.h"

namespace MoveHistory {
    class MoveHistory {
    public:
        void clear();

        void addPosition(U64 positionHash, bool isIrreversibleMove);

        void removeLastPosition();

        [[nodiscard]] bool isRepetition(U64 positionHash) const;

        [[nodiscard]] int pliesSinceIrreversibleMove() const;

    private:
        std::vector<U64> positionHashes;
        std::vector<int> lastIrreversibleIndex;
    };
} // MoveHistory

#endif //MOVEHISTORY_H
