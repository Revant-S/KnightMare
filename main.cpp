#include <bits/stdc++.h>
#include "board/Board.h"
#include "PreMatchComputations/PreMatchAttackComputation.h"
#include "tests/tests.h"


int main() {
    PreMatchAttackComputation::init();
    Board startPos;
    // tests::runPerftSuite(startPos, STARTING_POSITION_PERFT, "Start Position");
    Board kiwiPos(KIWIPETE_PERFT_START_FEN);
    tests::runPerftSuite(kiwiPos, KIWIPETE_PERFT, "KIWI POSITION");
}
