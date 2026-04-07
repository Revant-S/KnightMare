#include "utils.h"
#include "../types_constants/constants.h"
#include "../search/legal_move_generation/GeneratePseudoLegalMove.h"
#include <cctype>
#include <algorithm>

#include "../search/legal_move_generation/MoveFunctions.h"

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

    void printMoves(MoveList &moves) {
        for (auto &move: moves) {
            std::cout << moveToString(move);
            if (move.moveType != SIMPLE) {
                std::cout << " (type=" << move.moveType << ")";
            }
            std::cout << "\n";
        }
    }

    void printAllPseudoLegalMoves(Board &board) {
        auto printSection = [&](const std::string &label, MoveList moves) {
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

    void populatePromotionMoves(int pawnPosition, int destination, MoveList &moves, Color side) {
        for (int promoteTo = KNIGHT; promoteTo <= QUEEN; promoteTo++) {
            moves.addMove({
                pawnPosition,
                destination,
                side,
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

    MoveType getMoveType(const int from, const int to, Color color, Piece piece, Board &board) {
        // 1. Castling: A king moving exactly 2 squares
        if (piece == KING && abs(to - from) == 2) {
            if (to > from) {
                return CASTLE_KING_SIDE;
            }
            return CASTLE_QUEEN_SIDE;
        }
        int enPassantSquare = board.getEnpassantSquare();
        if (piece == PAWN && enPassantSquare != -1 && to == enPassantSquare) {
            if (from % 8 != to % 8) {
                return EN_PASSANT;
            }
        }
        if (piece == PAWN && abs(from - to) == 16) {
            return DOUBLE_PAWN_MOVE;
        }
        if (piece == PAWN && checkPawnPromotion(color, to)) {
            return PROMOTION;
        }
        return SIMPLE;
    }


    std::string trim(const std::string &s) {
        const auto begin = s.find_first_not_of(" \t\n\r\f\v");
        if (begin == std::string::npos) return "";
        const auto end = s.find_last_not_of(" \t\n\r\f\v");
        return s.substr(begin, end - begin + 1);
    }

    bool isValidFile(char c) {
        return c >= 'a' && c <= 'h';
    }

    bool isValidRank(char c) {
        return c >= '1' && c <= '8';
    }

    int squareFromUci(char fileChar, char rankChar) {
        int file = fileChar - 'a';
        int rank = rankChar - '1';
        return rank * 8 + file;
    }

    Piece promotionPieceFromChar(char c) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        switch (c) {
            case 'q': return QUEEN;
            case 'r': return ROOK;
            case 'b': return BISHOP;
            case 'n': return KNIGHT;
            default: return QUEEN; // fallback — caller should only pass valid chars
        }
    }

    Move parseMoveString(const std::string &moveStrRaw, Board &board) {
        const std::string moveStr = trim(moveStrRaw);

        const int from = squareFromUci(moveStr[0], moveStr[1]);
        const int to = squareFromUci(moveStr[2], moveStr[3]);

        auto [color, piece] = board.getPieceOnTheIndex(from);

        Piece promoteTo = PAWN;
        MoveType moveType = getMoveType(from, to, color, piece, board);

        if (moveStr.size() == 5) {
            moveType = PROMOTION;
            promoteTo = promotionPieceFromChar(moveStr[4]);
        }

        return Move{from, to, color, piece, moveType, promoteTo};
    }


    std::string getFenAfterMove(const std::string &fen, const std::string &move) {
        std::ofstream cmd("/tmp/sf_cmd.txt");
        cmd << "position fen " << fen << " moves " << move << "\n";
        cmd << "d\n"; // stockfish 'd' command prints the current FEN
        cmd << "quit\n";
        cmd.close();

        FILE *sf = popen("stockfish < /tmp/sf_cmd.txt", "r");
        char line[512];
        std::string result;
        while (fgets(line, sizeof(line), sf)) {
            std::string s(line);
            if (s.find("Fen:") != std::string::npos) {
                result = s.substr(5); // strip "Fen: "
                result.erase(result.find('\n'));
                break;
            }
        }
        pclose(sf);
        return result;
    }
}
