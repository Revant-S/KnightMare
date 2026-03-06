//
// Created by revant-sinha on 3/6/26.
//

#ifndef UTILS_H
#define UTILS_H
#include "types.h"

namespace Utils {
    void printBitBoard(const U64 bitBoard);

    SquareCoords getCoordinates(const int square);

    inline int getLSB(U64 bb) {
        if (bb == 0) return -1;
        return std::countr_zero(bb);
    }

    inline void popLSB(U64 &bb) {
        if (bb) bb &= (bb - 1);
    }
} // Utils

#endif //UTILS_H
