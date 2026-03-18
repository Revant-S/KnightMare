#include <bits/stdc++.h>
#include "board/Board.h"
#include "PreMatchComputations/PreMatchAttackComputation.h"
#include "search/legal_move_generation/MoveFunctions.h"
#include "utils/utils.h"

int main() {
    std::string startFen = "rnb1kb1r/pp2pppp/8/1Bp5/8/2P5/P1P3PP/RNBK2NR b kq - 1 9";
    Board board(startFen);
    // Board board{};
    PreMatchAttackComputation::init();
    std::cout << "--- BOARD STATE ---\n";
    board.print_board();
    Utils::printCastleRights(board);
    Utils::printMoves(board);
    bool flag = MoveFunctions::isKingInCheck(BLACK, board);
    if (flag) {
        std::cout << "YES CHECK IS THERE";
        return 0;
    }
    std::cout << "NO ";
    return 0;
}
