#include <bits/stdc++.h>
#include "board/Board.h"
#include "PreMatchComputations/PreMatchAttackComputation.h"
#include "utils/utils.h"

int main() {
    std::string startFen = "rn2kbnr/1P1bpppp/B7/p7/2p1p3/2N5/P1P2PPP/3RK1NR w Kkq - 1 13";
    Board board(startFen);
    // Board board{};
    PreMatchAttackComputation::init();
    std::cout << "--- BOARD STATE ---\n";
    board.print_board();
    Utils::printCastleRights(board);
    Utils::printMoves(board);

    return 0;
}
