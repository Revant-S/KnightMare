#include <vector>
#include <iostream>

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
}
