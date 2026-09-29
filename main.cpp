#include <bits/stdc++.h>
#include "board/Board.h"
#include "PreMatchComputations/PreMatchAttackComputation.h"
#include "History/MoveHistory.h"
#include "search/alpha_beta_search/Search.h"
#include "search/legal_move_generation/MoveFunctions.h"
#include "TranspositionTable/TransPositionTable.h"
#include "utils/utils.h"

void setPosition(const std::string &line, Board &board, MoveHistory::MoveHistory &history) {
    if (line.find("startpos") != std::string::npos) {
        board = Board(NORMAL_START_POSITION_FEN);
    } else if (line.find("fen") != std::string::npos) {
        std::string fen = line.substr(line.find("fen") + 4);
        if (fen.find(" moves") != std::string::npos)
            fen = fen.substr(0, fen.find(" moves"));
        board = Board(fen);
    }
    history.clear();
    history.addPosition(board.getPositionHash(), true);

    if (line.find("moves") == std::string::npos) return;
    std::istringstream movesStream(line.substr(line.find("moves") + 6));
    std::string moveString;
    while (movesStream >> moveString) {
        Move move = Utils::parseMoveString(moveString, board);
        const bool isIrreversible = move.piece == PAWN || MoveFunctions::isCapture(move, board);
        board.makeMove(move);
        board.toggle_side();
        history.addPosition(board.getPositionHash(), isIrreversible);
    }
}

Search::SearchLimits parseGoCommand(const std::string &line, const Board &board) {
    std::istringstream tokens(line);
    std::string token;
    long long whiteTime = -1, blackTime = -1, whiteIncrement = 0, blackIncrement = 0, moveTime = -1;
    int movesToGo = 0;
    Search::SearchLimits limits;
    while (tokens >> token) {
        if (token == "wtime") tokens >> whiteTime;
        else if (token == "btime") tokens >> blackTime;
        else if (token == "winc") tokens >> whiteIncrement;
        else if (token == "binc") tokens >> blackIncrement;
        else if (token == "movestogo") tokens >> movesToGo;
        else if (token == "movetime") tokens >> moveTime;
        else if (token == "depth") tokens >> limits.maxDepth;
    }

    const bool isWhite = board.getSide() == WHITE;
    const long long remainingTime = isWhite ? whiteTime : blackTime;
    const long long increment = isWhite ? whiteIncrement : blackIncrement;
    if (moveTime > 0) limits.timeBudgetMs = moveTime;
    else if (remainingTime >= 0) limits.timeBudgetMs = Search::computeTimeBudget(remainingTime, increment, movesToGo);
    return limits;
}

void uciLoop() {
    Board board;
    MoveHistory::MoveHistory history;
    history.addPosition(board.getPositionHash(), true);
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line == "uci") {
            std::cout << "id name KnightMare\n";
            std::cout << "id author Revant Sinha\n";
            std::cout << "uciok\n";
        } else if (line == "isready") {
            std::cout << "readyok\n";
        } else if (line == "ucinewgame") {
            board = Board(NORMAL_START_POSITION_FEN);
            TranspositionTable::clear();
        } else if (line.starts_with("position")) {
            setPosition(line, board, history);
        } else if (line.starts_with("go")) {
            const Move best = Search::getBestMove(board, history, parseGoCommand(line, board));
            std::cout << "bestmove " << Utils::moveToString(best) << "\n";
        } else if (line == "d") {
            board.print_board();
        } else if (line == "quit") {
            break;
        }
        std::cout.flush();
    }
}

int main() {
    PreMatchAttackComputation::init();
    uciLoop();
}
