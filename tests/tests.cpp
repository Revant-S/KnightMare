#include <vector>
#include <iostream>
#include <cstdio>
#include <string>
#include <map>
#include <sstream>
#include "../search/legal_move_generation/MoveFunctions.h"
#include "../types_constants/types.h"
#include "../utils/utils.h"

namespace tests {
    void visualizeMoves(const std::vector<Move> &moves) {
        U64 attackMap = 0ULL;
        int pieceSquare = -1;

        for (const auto &move: moves) {
            attackMap |= (1ULL << move.to);
            pieceSquare = move.from;
        }

        if (pieceSquare != -1) {
            std::cout << "Found " << moves.size() << " pseudo-legal moves for piece on square " << pieceSquare << ":";
            Utils::printBitBoard(attackMap);
        } else {
            std::cout << "No moves found or piece is completely blocked.\n";
        }
    }

    int perft(Board &board, int depth) {
        if (depth == 0) return 1;
        std::vector<Move> legalMoves = MoveFunctions::getAllLegalMoves(board);
        if (legalMoves.empty()) return 0;
        if (depth == 1) return legalMoves.size(); // bulk count, skip makeMove

        int nodes = 0;
        for (auto &move: legalMoves) {
            BoardState saved = board.saveState();
            board.makeMove(move, board.getSide());
            board.toggle_side();
            nodes += perft(board, depth - 1);
            board.unmakeMove(saved);
        }
        return nodes;
    }

    void perftDivide(Board &board, int depth) {
        auto allLegalMoves = MoveFunctions::getAllLegalMoves(board);

        for (auto &move: allLegalMoves) {
            // std::cout << Utils::moveToString(move) << "\n";
            BoardState saved = board.saveState();
            board.makeMove(move, board.getSide());
            board.toggle_side();
            int nodes = perft(board, depth - 1);
            board.unmakeMove(saved);
            std::cout << Utils::moveToString(move) << ": " << nodes << " " << moveTypeString[move.moveType] << "\n";
        }
    }

    void runPerftSuite(Board &board, const std::vector<PerftResult> &expectedResults, const std::string &suiteName) {
        std::cout << "\n=== Perft Suite: " << suiteName << " ===\n";

        for (const auto &[depth, expected]: expectedResults) {
            long long result = perft(board, depth);
            bool passed = result == expected;

            std::cout << "Depth " << depth << ": "
                    << (passed ? "PASS" : "FAIL")
                    << "  got=" << result
                    << "  expected=" << expected;

            if (!passed) {
                std::cout << "  delta=" << (result - expected) << "\n";
                std::cout << "Stopping — fix depth " << depth << "\n";
                perft(board, depth); // print per-move breakdown to isolate the bug
                return;
            }
            std::cout << "\n";
        }

        std::cout << "All depths passed.\n";
    }


    std::map<std::string, int> getStockfishPerft(const std::string &fen, int depth) {
        std::ofstream cmdFile("/tmp/sf_commands.txt");
        cmdFile << "position fen " << fen << "\n";
        cmdFile << "go perft " << depth << "\n";
        cmdFile << "quit\n";
        cmdFile.close();
        FILE *sf = popen("stockfish < /tmp/sf_commands.txt", "r");
        if (!sf) {
            std::cerr << "Failed to open stockfish\n";
            return {};
        }

        std::map<std::string, int> results;
        char line[256];
        while (fgets(line, sizeof(line), sf)) {
            std::string s(line);
            auto colonPos = s.find(": ");
            if (colonPos != std::string::npos && s.find("Nodes") == std::string::npos) {
                std::string move = s.substr(0, colonPos);
                int count = std::stoi(s.substr(colonPos + 2));
                results[move] = count;
            }
            if (s.find("Nodes searched") != std::string::npos) break;
        }

        pclose(sf);
        return results;
    }

    void compareWithStockfish(Board &board, const std::string &fen, int depth) {
        auto sfResults = getStockfishPerft(fen, depth);
        std::map<std::string, int> myResults;
        for (auto &move: MoveFunctions::getAllLegalMoves(board)) {
            BoardState saved = board.saveState();
            board.makeMove(move, board.getSide());
            board.toggle_side();
            int nodes = tests::perft(board, depth - 1);
            board.unmakeMove(saved);
            myResults[Utils::moveToString(move)] = nodes;
        }
        std::cout << "\n=== Comparison at depth " << depth << " ===\n";
        bool found = false;
        for (auto &[move, sfCount]: sfResults) {
            int myCount = myResults.contains(move) ? myResults[move] : -1;
            if (myCount != sfCount) {
                std::cout << "MISMATCH: " << move
                        << "  mine=" << myCount
                        << "  stockfish=" << sfCount
                        << "  delta=" << (myCount - sfCount) << "\n";
                found = true;
            }
        }
        for (auto &[move, myCount]: myResults) {
            if (!sfResults.count(move)) {
                std::cout << "EXTRA MOVE (not in stockfish): " << move
                        << " count=" << myCount << "\n";
                found = true;
            }
        }
        if (!found) std::cout << "All moves match!\n";
    }

    void isoLateIncorrectMove(Board board, const std::string &fen, int depth) {
        if (depth == 0) return;

        auto sfResults = getStockfishPerft(fen, depth);

        // build per-move map same as compareWithStockfish
        std::map<std::string, int> myResults;
        for (auto &move: MoveFunctions::getAllLegalMoves(board)) {
            BoardState saved = board.saveState();
            board.makeMove(move, board.getSide());
            board.toggle_side();
            int nodes = perft(board, depth - 1);
            board.unmakeMove(saved);
            myResults[Utils::moveToString(move)] = nodes;
        }

        for (auto &[move, sfCount]: sfResults) {
            int myCount = myResults.count(move) ? myResults[move] : 0;
            if (myCount != sfCount) {
                std::cout << "depth=" << depth
                        << " bad move=" << move
                        << " mine=" << myCount
                        << " stockfish=" << sfCount
                        << " delta=" << (myCount - sfCount) << "\n";

                Move m = Utils::parseMoveString(move, board);
                BoardState saved = board.saveState();
                board.makeMove(m, board.getSide());
                board.toggle_side();

                std::string newFen = Utils::getFenAfterMove(fen, move);
                isoLateIncorrectMove(board, newFen, depth - 1);
                board.unmakeMove(saved);
                return;
            }
        }
        std::cout << "depth=" << depth << " all moves match\n";
    }

    void testAllPositions() {
        Board p4(POSITION4_FEN);
        Board p3(POSITION3_FEN);
        Board p5(POSITION5_FEN);
        Board startPos;
        Board kiwiPos(KIWIPETE_PERFT_START_FEN);
        runPerftSuite(p4, POSITION4_PERFT, "Position 4");
        runPerftSuite(p3, POSITION3_PERFT, "Position 3");
        runPerftSuite(p5, POSITION5_PERFT, "Position 5");
        runPerftSuite(startPos, STARTING_POSITION_PERFT, "START POSITION");
        runPerftSuite(kiwiPos, KIWIPETE_PERFT, "KIWI POSITION");
    }
}
