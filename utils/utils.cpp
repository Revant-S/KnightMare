//
// Created by revant-sinha on 3/6/26.
//

#include "utils.h"
#include "../types_constants/constants.h"

namespace Utils {
    void printBitBoard(U64 bitBoard) {
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


    SquareCoords getCoordinates(int square) {
        return {square / 8, square % 8};
    }

    void compareFen(std::string &fenGenerated , std::string &fenToCompare) {
        if (fenGenerated == fenToCompare) {
            std::cout << "Two strings match";
            return;
        }
        const int n = fenGenerated.size();
        for (int i = 0; i < n; i++) {
            if (fenGenerated[i] != fenToCompare[i]) {
                std::cout << "Generated String contains " << fenGenerated[i] << " On Index " << i <<
                        " but required is: " << fenToCompare[i] << "\n";
            }
        }
    }
}
