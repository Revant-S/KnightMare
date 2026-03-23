#include <bits/stdc++.h>
#include "board/Board.h"
#include "PreMatchComputations/PreMatchAttackComputation.h"
#include "tests/tests.h"

int main() {
    PreMatchAttackComputation::init();
    Board startPos;
    // tests::runPerftSuite(startPos, STARTING_POSITION_PERFT, "Starting Position");
    Board kiwiBoard(KIWIPETE_PERFT_START_FEN);
    // tests::runPerftSuite(kiwiBoard, KIWIPETE_PERFT, "KIWIPETE Position");
    // tests::perftDivide(kiwiBoard, 2);
}
