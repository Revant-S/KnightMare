//
// Created by revant-sinha on 3/7/26.
//

#include "tests.h"
#include<iostream>

#include "MoveGeneration.h"
#include "utils.h"

void tests::checkAttackPositions(int testSquare) {
    std::cout << "--- KNIGHT  ---\n";
    Utils::printBitBoard(MoveGeneration::knightAttacks[testSquare]);

    std::cout << "\n--- BISHOP  ---\n";
    Utils::printBitBoard(MoveGeneration::bishopAttacks[testSquare]);

    std::cout << "\n--- ROOK ---\n";
    Utils::printBitBoard(MoveGeneration::rookAttacks[testSquare]);

    std::cout << "\n--- QUEEN ---\n";
    Utils::printBitBoard(MoveGeneration::queenAttacks[testSquare]);

    std::cout << "\n--- KING  ---\n";
    Utils::printBitBoard(MoveGeneration::kingAttacks[testSquare]);
}
