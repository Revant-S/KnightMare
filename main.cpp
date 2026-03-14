#include <bits/stdc++.h>
#include "board/Board.h"
#include "PreMatchComputations/PreMatchAttackComputation.h"
#include "search/legal_move_generation/GenerateLeglMove.h"
#include "utils/utils.h"

int main() {
    std::string startFen = "rnb1kbnr/2p1pppp/1p6/1P1q4/p4P2/N7/P1p1PKPP/R1BQ1BNR b kq - 7 12    ";
    Board board(startFen);
    PreMatchAttackComputation::init();
    std::cout << "--- BOARD STATE ---\n";
    board.print_board();
    Utils::printCastleRights(board);
    auto printMoves = [](std::vector<Move> &moves) {
        for (auto &move: moves) {
            SquareCoords from = Utils::getCoordinates(move.from);
            SquareCoords to = Utils::getCoordinates(move.to);

            char fromFile = 'a' + from.file;
            char toFile = 'a' + to.file;

            std::cout << fromFile << from.rank + 1
                    << " -> "
                    << toFile << to.rank + 1
                    << "\n";
        }
    };


    std::cout << "\n--- KNIGHT MOVES ---\n";
    auto knightMoves = GenerateLegalMove::getKnightPseudoLegalMoves(board);
    printMoves(knightMoves);


    std::cout << "\n--- ROOK MOVES ---\n";
    auto rookMoves = GenerateLegalMove::getRookPseudoLegalMoves(board);
    printMoves(rookMoves);


    std::cout << "\n--- BISHOP MOVES ---\n";
    auto bishopMoves = GenerateLegalMove::getBishopPseudoLegalMoves(board);
    printMoves(bishopMoves);

    std::cout << "\n--- QUEEN MOVES ---\n";
    auto queenMoves = GenerateLegalMove::getQueenPseudoLegalMoves(board);
    printMoves(queenMoves);


    std::cout << "\n--- KING MOVES ---\n";
    auto kingMoves = GenerateLegalMove::getKingPseudoLegalMoves(board);
    printMoves(kingMoves);

    std::cout << "\n--- Pawn MOVES ---\n";
    auto pawnMoves = GenerateLegalMove::getPawnPseudoLegalMoves(board);
    printMoves(pawnMoves);

    return 0;
}
