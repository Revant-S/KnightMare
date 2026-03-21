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
                std::cout << (bitBoard & position ? "1 " : ". ");
            }
            std::cout << "\n";
        }
        std::cout << "   a b c d e f g h\n\n";
    }

    SquareCoords getCoordinates(int square) {
        return {square / 8, square % 8};
    }

    std::string squareToString(int square) {
        SquareCoords coords = getCoordinates(square);
        return std::string(1, 'a' + coords.file) + std::to_string(coords.rank + 1);
    }

    std::string moveToString(const Move &move) {
        static const char *promotionPieceChar = {"nbrq"};

        std::string result = squareToString(move.from) + squareToString(move.to);
        if (move.moveType == PROMOTION) {
            result += promotionPieceChar[move.promoteTo - KNIGHT];
        }
        return result;
    }

    void printMoves(std::vector<Move> &moves) {
        for (auto &move: moves) {
            std::cout << moveToString(move);
            if (move.moveType != SIMPLE) {
                std::cout << " (type=" << move.moveType << ")";
            }
            std::cout << "\n";
        }
    }

    void printAllPseudoLegalMoves(Board &board) {
        auto printSection = [&](const std::string &label, std::vector<Move> moves) {
            std::cout << "\n--- " << label << " ---\n";
            printMoves(moves);
        };

        printSection("KNIGHT", GeneratePseudoLegalMove::getKnightPseudoLegalMoves(board));
        printSection("ROOK", GeneratePseudoLegalMove::getRookPseudoLegalMoves(board));
        printSection("BISHOP", GeneratePseudoLegalMove::getBishopPseudoLegalMoves(board));
        printSection("QUEEN", GeneratePseudoLegalMove::getQueenPseudoLegalMoves(board));
        printSection("KING", GeneratePseudoLegalMove::getKingPseudoLegalMoves(board));
        printSection("PAWN", GeneratePseudoLegalMove::getPawnPseudoLegalMoves(board));

        std::cout << "\n--- EN PASSANT SQUARE ---\n";
        int ep = board.getEnpassantSquare();
        std::cout << (ep == -1 ? "none" : squareToString(ep)) << "\n";
    }

    void printCastleRights(Board &board) {
        int white = board.getCastleRights(WHITE);
        int black = board.getCastleRights(BLACK);

        if (white & WHITE_KING_SIDE_CASTLE_MASK) std::cout << "White can castle kingside\n";
        if (white & WHITE_QUEEN_SIDE_CASTLE_MASK) std::cout << "White can castle queenside\n";
        if (black & BLACK_KING_SIDE_CASTLE_MASK) std::cout << "Black can castle kingside\n";
        if (black & BLACK_QUEEN_SIDE_CASTLE_MASK) std::cout << "Black can castle queenside\n";
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

    void compareFen(const std::string &fenGenerated, const std::string &fenToCompare) {
        if (fenGenerated == fenToCompare) {
            std::cout << "FEN strings match\n";
            return;
        }
        const int n = std::min(fenGenerated.size(), fenToCompare.size());
        for (int i = 0; i < n; i++) {
            if (fenGenerated[i] != fenToCompare[i]) {
                std::cout << "Mismatch at index " << i
                        << ": got='" << fenGenerated[i]
                        << "' expected='" << fenToCompare[i] << "'\n";
            }
        }
        if (fenGenerated.size() != fenToCompare.size()) {
            std::cout << "Length mismatch: got=" << fenGenerated.size()
                    << " expected=" << fenToCompare.size() << "\n";
        }
    }
}
