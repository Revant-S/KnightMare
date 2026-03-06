//
// Created by revant-sinha on 3/6/26.
//

#include "utils.h"
#include "constants.h"

namespace Utils {
    void printBitBoard(const U64 bitBoard) {
        std::cout << "\n";
        for (int rank = 7; rank >= 0; rank--) {
            std::cout << rank + 1 << "  ";
            for (int file = 0; file < 8; file++) {
                U64 position = 1ULL << (rank * 8 + file);
                if (bitBoard & position) {
                    std::cout << "1 ";
                } else {
                    std::cout << ". ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "   a b c d e f g h\n\n";
    }


    SquareCoords getCoordinates(const int square) {
        return {square / 8, square % 8};
    }
}
