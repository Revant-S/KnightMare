#include <bits/stdc++.h>
#include "board/Board.h"
#include "PreMatchComputations/PreMatchAttackComputation.h"
#include "utils/utils.h"

int main() {
    std::string startFen = "rnbqkbnr/p2p1ppp/3P4/1pp5/2P1pP2/8/PP2P1PP/RNBQKBNR b KQkq f3 0 5";
    Board board(startFen);
    // Board board{};
    PreMatchAttackComputation::init();
    std::cout << "--- BOARD STATE ---\n";
    board.print_board();
    Utils::printCastleRights(board);
    Utils::printMoves(board);

    return 0;
}
