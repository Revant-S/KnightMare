#include <bits/stdc++.h>
#include "board.h"
#include "movegen.h"
#include "utils.h"

int main() {
    MoveGen::generateKnightAttacks();
    MoveGen::generateBishopAttacks();
    MoveGen::generateRookAttacks();
    MoveGen::generateQueenAttacks();
    MoveGen::generateKingAttacks();
    int testSquare = 28;
    std::cout << "--- KNIGHT on e4 ---\n";
    Utils::printBitBoard(MoveGen::knightAttacks[testSquare]);

    std::cout << "\n--- BISHOP on e4 ---\n";
    Utils::printBitBoard(MoveGen::bishopAttacks[testSquare]);

    std::cout << "\n--- ROOK on e4 ---\n";
    Utils::printBitBoard(MoveGen::rookAttacks[testSquare]);

    std::cout << "\n--- QUEEN on e4 ---\n";
    Utils::printBitBoard(MoveGen::queenAttacks[testSquare]);

    std::cout << "\n--- KING on e4 ---\n";
    Utils::printBitBoard(MoveGen::kingAttacks[testSquare]);

    return 0;
}
