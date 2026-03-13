#include <vector>
#include <iostream>
#include "../types_constants/types.h"
#include "../utils/utils.h"

namespace tests {
    void visualizeMoves(const std::vector<Move> &moves) {
        U64 attackMap = 0ULL;
        int pieceSquare = -1;

        for (const auto &move: moves) {
            attackMap |= (1ULL << move.to); // Turn on the bit for every legal destination
            pieceSquare = move.from; // Grab the starting square for logging
        }

        if (pieceSquare != -1) {
            std::cout << "Found " << moves.size() << " pseudo-legal moves for piece on square " << pieceSquare << ":";
            Utils::printBitBoard(attackMap);
        } else {
            std::cout << "No moves found or piece is completely blocked.\n";
        }
    }
}
