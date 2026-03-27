#include <bits/stdc++.h>
#include "board/Board.h"
#include "PreMatchComputations/PreMatchAttackComputation.h"
#include "tests/tests.h"


int main() {
    PreMatchAttackComputation::init();
    Board p4(POSITION4_FEN);
    tests::runPerftSuite(p4, POSITION4_PERFT, "Position 4");
    // tests::isoLateIncorrectMove(p4,POSITION4_FEN,3);
    Board p5(POSITION5_FEN);
    tests::runPerftSuite(p5, POSITION5_PERFT, "Position 5");
}