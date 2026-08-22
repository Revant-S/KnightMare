//
// Created by revant-sinha on 4/28/26.
//

#include "MoveHistory.h"

namespace MoveHistory {
    void MoveHistory::clear() {
        positionHashes.clear();
        lastIrreversibleIndex.clear();
    }

    void MoveHistory::addPosition(const U64 positionHash, const bool isIrreversibleMove) {
        const int currentIndex = static_cast<int>(positionHashes.size());
        const int previousBoundary = lastIrreversibleIndex.empty() ? 0 : lastIrreversibleIndex.back();
        lastIrreversibleIndex.push_back(isIrreversibleMove ? currentIndex : previousBoundary);
        positionHashes.push_back(positionHash);
    }

    void MoveHistory::removeLastPosition() {
        positionHashes.pop_back();
        lastIrreversibleIndex.pop_back();
    }

    bool MoveHistory::isRepetition(const U64 positionHash) const {
        if (positionHashes.empty()) return false;
        const int boundary = lastIrreversibleIndex.back();
        for (int index = static_cast<int>(positionHashes.size()) - 3; index >= boundary; index -= 2) {
            if (positionHashes[index] == positionHash) return true;
        }
        return false;
    }

    int MoveHistory::pliesSinceIrreversibleMove() const {
        if (positionHashes.empty()) return 0;
        return static_cast<int>(positionHashes.size()) - 1 - lastIrreversibleIndex.back();
    }
} // MoveHistory
