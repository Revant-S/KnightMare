#include <bits/stdc++.h>
#include "board/Board.h"
#include "PreMatchComputations/PreMatchAttackComputation.h"
#include "utils/utils.h"

int main() {
    std::string startFen = "rnbqkbnr/pPp1pppp/8/8/8/8/PP1PpPPP/RNBQKBNR w KQkq - 0 5";
    Board board(startFen);
    // Board board{};
    PreMatchAttackComputation::init();
    std::cout << "--- BOARD STATE ---\n";
    board.print_board();
    Utils::printCastleRights(board);
    Utils::printMoves(board);

    return 0;
}
