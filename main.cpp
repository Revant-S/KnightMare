#include <bits/stdc++.h>
#include "board/Board.h"
#include "PreMatchComputations/PreMatchAttackComputation.h"
#include "tests/tests.h"


int main() {
    PreMatchAttackComputation::init();
    std::string kiwiFen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";
    Board kiwiBoard(kiwiFen);
    tests::runPerftSuite(kiwiBoard, KIWIPETE_PERFT, "KIWI");
    // tests::compareWithStockfish(kiwiBoard , kiwiFen , 4);
    // Board startPos;
    // tests::runPerftSuite(startPos, STARTING_POSITION_PERFT, "Start Position");

}
