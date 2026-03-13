#include <bits/stdc++.h>
#include "board/Board.h"
#include "PreMatchComputations/PreMatchAttackComputation.h"
#include "search/legal_move_generation/GenerateLeglMove.h"
#include "tests/tests.h"
#include "utils/utils.h"

int main() {
    std::string startFen = "rnb1kbnr/pp1ppppp/8/q1p5/2P1P3/8/PP1P1PPP/RNBQKBNR w kq- - 1 3";
    Board board(startFen);
    PreMatchAttackComputation::init();

    std::cout << "--- BOARD STATE ---\n";
    board.print_board();
    Utils::printCastleRights(board);


    return 0;
}
