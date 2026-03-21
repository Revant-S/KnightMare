#include <bits/stdc++.h>
#include "board/Board.h"
#include "PreMatchComputations/PreMatchAttackComputation.h"
#include "search/legal_move_generation/GeneratePseudoLegalMove.h"

#include "tests/tests.h"
#include "utils/utils.h"

int main() {
    PreMatchAttackComputation::init();

    Board startPos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    tests::runPerftSuite(startPos, STARTING_POSITION_PERFT, "Starting Position");

    Board kiwipete("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K22 w KQkq -");
    tests::runPerftSuite(kiwipete, KIWIPETE_PERFT, "Kiwipete");
}
