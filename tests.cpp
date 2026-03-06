//
// Created by revant-sinha on 3/7/26.
//

#include "tests.h"
#include<iostream>

#include "AttackGen.h"
#include "utils.h"

void tests::checkAttackPositions(int testSquare) {
    std::cout << "--- KNIGHT  ---\n";
    Utils::printBitBoard(AttackGen::knightAttacks[testSquare]);

    std::cout << "\n--- BISHOP  ---\n";
    Utils::printBitBoard(AttackGen::bishopAttacks[testSquare]);

    std::cout << "\n--- ROOK ---\n";
    Utils::printBitBoard(AttackGen::rookAttacks[testSquare]);

    std::cout << "\n--- QUEEN ---\n";
    Utils::printBitBoard(AttackGen::queenAttacks[testSquare]);

    std::cout << "\n--- KING  ---\n";
    Utils::printBitBoard(AttackGen::kingAttacks[testSquare]);
}
