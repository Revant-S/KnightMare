//
// Created by revant-sinha on 3/6/26.
//

#ifndef UTILS_H
#define UTILS_H
#include "../board/Board.h"
#include "../types_constants/types.h"

namespace Utils {
    void printBitBoard(U64 bitBoard);

    void compareFen(std::string &fenGenerated, std::string &fenToCompare);

    SquareCoords getCoordinates(int square);

    void printCastleRights(Board &board);


    inline int getLSB(const U64 bb) {
        assert(bb != 0);
        return std::countr_zero(bb);
    }

    inline int getMSB(const U64 bb) {
        assert(bb != 0);
        return 63 - __builtin_clzll(bb);
    }

    inline void popLSB(U64 &bb) {
        if (bb) bb &= (bb - 1);
    }
} // Utils

#endif //UTILS_H
