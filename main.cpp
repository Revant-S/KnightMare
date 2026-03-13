#include <bits/stdc++.h>
#include "board/Board.h"
#include "PreMatchComputations/PreMatchAttackComputation.h"
#include "search/legal_move_generation/GenerateLeglMove.h"
#include "tests/tests.h"

int main() {
    Board board;
    PreMatchAttackComputation::init();

    std::cout << "--- BOARD STATE ---\n";
    board.print_board();
    //
    // std::cout << "\n--- TESTING WHITE KNIGHT MOVES ---\n";
    // // Get all White Knight moves
    // std::vector<Move> knightMoves = GenerateLegalMove::getBishopPseudoLegalMoves(board);
    //
    // // Visualize them!
    // tests::visualizeMoves(knightMoves);

    board.getAndPrintPawnMovePermissions();

    return 0;
}
