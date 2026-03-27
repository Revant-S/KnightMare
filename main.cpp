#include <bits/stdc++.h>
#include "board/Board.h"
#include "PreMatchComputations/PreMatchAttackComputation.h"
#include "tests/tests.h"


int main() {
    PreMatchAttackComputation::init();
    tests::testAllPositions();
}