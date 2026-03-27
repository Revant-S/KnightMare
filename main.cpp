#include <bits/stdc++.h>
#include "board/Board.h"
#include "PreMatchComputations/PreMatchAttackComputation.h"
#include "tests/tests.h"


int main() {
    PreMatchAttackComputation::init();
    // std::string kiwiFen = "r3k2r/p1ppqQbn/bn2p1p1/3PN3/1p2P3/2N4p/PPPBBPPP/1R2K2R b Kkq - 0 2";
    // std::string kiwiFen = "r3k2r/p1ppqNb1/1n2pnp1/1b1P4/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 1 2";
    // Board kiwiBoard(KIWIPETE_PERFT_START_FEN);
    // Board kiwiActualBoard(KIWIPETE_PERFT_START_FEN);
    // tests::runPerftSuite(kiwiBoard, KIWIPETE_PERFT, "KIWI");
    // tests::compareWithStockfish(kiwiBoard, KIWIPETE_PERFT_START_FEN, 4);
    Board startPos;
    tests::runPerftSuite(startPos, STARTING_POSITION_PERFT, "Start Position");
}
