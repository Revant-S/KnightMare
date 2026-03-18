//
// Created by revant-sinha on 3/6/26.
//

#include "utils.h"
#include "../types_constants/constants.h"
#include "../search/legal_move_generation/GeneratePseudoLegalMove.h"

namespace Utils {
    void printBitBoard(U64 bitBoard) {
        std::cout << "\n";
        for (int rank = 7; rank >= 0; rank--) {
            std::cout << rank + 1 << "  ";
            for (int file = 0; file < 8; file++) {
                U64 position = 1ULL << (rank * 8 + file);
                if (bitBoard & position) {
                    std::cout << "1 ";
                } else {
                    std::cout << ". ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "   a b c d e f g h\n\n";
    }


    SquareCoords getCoordinates(int square) {
        return {square / 8, square % 8};
    }


    void printCastleRights(Board &board) {
        int whiteCastleRights = board.getCastleRights(WHITE);
        int blackCastleRights = board.getCastleRights(BLACK);

        if (whiteCastleRights & WHITE_KING_SIDE_CASTLE_MASK) {
            std::cout << "White can castle kingside\n";
        }

        if (whiteCastleRights & WHITE_QUEEN_SIDE_CASTLE_MASK) {
            std::cout << "White can castle queenside\n";
        }

        if (blackCastleRights & BLACK_KING_SIDE_CASTLE_MASK) {
            std::cout << "Black  can castle kingside\n";
        }

        if (blackCastleRights & BLACK_QUEEN_SIDE_CASTLE_MASK) {
            std::cout << "Black  can castle queenside\n";
        }
    }

    void printMoves(Board &board) {
        auto printMoves = [](std::vector<Move> &moves) {
            for (auto &move: moves) {
                std::cout << "Move Type : " << move.moveType << " ";
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

        std::cout << "\n --- En passant Square ---\n ";
        std::cout << board.getEnpassantSquare();
    }

    void populatePromotionMoves(int pawnPosition, int destination, std::vector<Move> &moves) {
        for (int promoteTo = KNIGHT; promoteTo <= QUEEN; promoteTo++) {
            moves.push_back({
                pawnPosition,
                destination,
                PAWN,
                PROMOTION,
                static_cast<Piece>(promoteTo)

            });
        }
    }


    void compareFen(std::string &fenGenerated, std::string &fenToCompare) {
        if (fenGenerated == fenToCompare) {
            std::cout << "Two strings match";
            return;
        }
        const int n = fenGenerated.size();
        for (int i = 0; i < n; i++) {
            if (fenGenerated[i] != fenToCompare[i]) {
                std::cout << "Generated String contains " << fenGenerated[i] << " On Index " << i <<
                        " but required is: " << fenToCompare[i] << "\n";
            }
        }
    }
}
