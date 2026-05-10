#include <bits/stdc++.h>
#include "board/Board.h"
#include "PreMatchComputations/PreMatchAttackComputation.h"
#include "search/alpha_beta_search/Search.h"
#include "search/evaluation/EvaluationUtils.h"
#include "utils/utils.h"

void uciLoop() {
    Board board;
    std::string line;

    while (std::getline(std::cin, line)) {
        std::cerr << "received: " << line << "\n";
        std::cerr.flush();

        if (line == "uci") {
            std::cout << "id name KnightMare\n";
            std::cout << "id author Revant Sinha\n";
            std::cout << "uciok\n";
            std::cout.flush();
        } else if (line == "isready") {
            std::cout << "readyok\n";
            std::cout.flush();
        } else if (line == "ucinewgame") {
            board = Board(NORMAL_START_POSITION_FEN);
            std::cerr << "NEW GAME";
        } else if (line.starts_with("position")) {
            if (line.find("startpos") != std::string::npos) {
                board = Board(NORMAL_START_POSITION_FEN);
            } else if (line.find("fen") != std::string::npos) {
                std::string fen = line.substr(line.find("fen") + 4);
                if (fen.find(" moves") != std::string::npos)
                    fen = fen.substr(0, fen.find(" moves"));
                board = Board(fen);
                std::cerr << "SIDE TO PLAY : \n" << board.getSide();
            }
            if (line.find("moves") != std::string::npos) {
                std::string movesStr = line.substr(line.find("moves") + 6);
                std::istringstream ss(movesStr);
                std::string moveStr;
                while (ss >> moveStr) {
                    Move move = Utils::parseMoveString(moveStr, board);
                    std::cerr << "applying: " << moveStr
                            << " from=" << move.from
                            << " to=" << move.to
                            << " piece=" << move.piece
                            << " color=" << move.colorOfPieceToMove << "\n";
                    std::cerr.flush();
                    board.makeMove(move);
                    board.toggle_side();
                }
            }
            std::cerr << "board after position:\n";
            board.print_board();
        } else if (line.starts_with("go")) {
            Move best = Search::getBestMove(board);
            std::cerr << "bestmove: " << Utils::moveToString(best) << "\n";
            std::cerr.flush();
            std::cout << "bestmove " << Utils::moveToString(best) << "\n";
            std::cout.flush();
        } else if (line == "quit") {
            break;
        }
    }
}

int main() {
    PreMatchAttackComputation::init();
    uciLoop();
    // Board board("r1bqkbnr/pppppppp/2n5/8/8/2N5/PPPPPPPP/R1BQKBNR w KQkq - 2 2");
    // Move move = Search::getBestMove(board);
    // std::cout << "Best Move :- \n";
    // // EvaluationUtils::printScoreBreakDown(board, move);
    // std::cout << "\n";
}
