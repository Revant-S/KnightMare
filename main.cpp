#include <bits/stdc++.h>
#include "board/Board.h"
#include "PreMatchComputations/PreMatchAttackComputation.h"
#include "search/alpha_beta_search/Search.h"
#include "tests/tests.h"
#include "utils/utils.h"

void startGame() {
    // main game loop
    std::string startFenString;
    std::cout << "Enter the FEN string you want to load. If None press enter\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear buffer
    std::getline(std::cin, startFenString);
    std::cout << startFenString;
    if (startFenString.empty()) {
        std::cout << "Loading default position...\n";
        startFenString = NORMAL_START_POSITION_FEN;
    } else {
        std::cout << "Loaded FEN: " << startFenString << "\n";
    }
    Board board(startFenString);
    board.print_board();
    int botColor;
    bool isValidColor = false;
    while (!isValidColor) {
        std::cout << "WHICH COLOR IS BOT PLAYING: 0-->WHITE 1--> BLACK";
        std::cin >> botColor;
        if (botColor == 1 || botColor == 0) {
            std::cout << botColor;
            isValidColor = true;
        }
    }

    while (true) {
        if (board.getSide() == static_cast<Color>(botColor)) {
            Move bestMove = Search::getBestMove(board);
            board.makeMove(bestMove);
            std::cout << "BOT MOVE: ";
            std::cout << Utils::moveToString(bestMove) << "\n";
        } else {
            std::string OpponentMove;
            std::cout << "Enter the your Move\n";
            std::cin >> OpponentMove;
            Move move = Utils::parseMoveString(OpponentMove, board);
            board.makeMove(move);
        }
        board.toggle_side();
    }
}

int main() {
    PreMatchAttackComputation::init();
    // tests::testAllPositions();
    startGame();
}
