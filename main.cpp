#include <bits/stdc++.h>
#include "board/Board.h"
#include "PreMatchComputations/PreMatchAttackComputation.h"
#include "utils/utils.h"

int main() {
    std::string startFen = "rnbqkb1r/pp1p1ppp/5n2/2p1p3/4P3/3B1N2/PPPP1PPP/RNBQK2R w KQkq - 0 4";
    Board board(startFen);
    // Board board{};
    PreMatchAttackComputation::init();
    std::cout << "--- BOARD STATE ---\n";
    board.print_board();
    Utils::printCastleRights(board);
    Utils::printMoves(board);

    return 0;
}
