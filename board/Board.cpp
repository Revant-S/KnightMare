//
// Created by revant-sinha on 3/6/26.
//

#include "Board.h"
#include "../types_constants/types.h"
#include <bitset>

Board::Board(const std::string &fenString) {
    int rank = 7, file = 0;
    int sideIndex = 0;
    for (const unsigned char piece: fenString) {
        sideIndex++;
        if (piece == ' ')break;
        if (piece == '/') {
            rank -= 1;
            file = 0;
        } else if (std::isdigit(piece)) {
            const int skips = piece - '0';
            file += skips;
        } else {
            int boardIndex = rank * 8 + file;
            if (boardIndex >= 0 && boardIndex < 64) {
                const int color = fenEnumMapping[piece].color;
                const int boardPiece = fenEnumMapping[piece].piece;
                bitboards[color][boardPiece] |= (static_cast<U64>(1) << boardIndex);
                occupancies[color] |= (static_cast<U64>(1) << boardIndex);
                occupancies[BOTH] |= (static_cast<U64>(1) << boardIndex);
                file++;
            } else {
                break;
            }
        }
    }
    if (fenString[sideIndex] == 'w') {
        side = WHITE;
    } else {
        side = BLACK;
    }
    int castleIndex = sideIndex + 2;

    castleRights = 0b0000;
    while (castleIndex < fenString.length() && fenString[castleIndex] != ' ') {
        if (const char symbol = fenString[castleIndex]; symbol == '-') {
            break;
        } else if (symbol == 'K') {
            castleRights |= WHITE_KING_SIDE_CASTLE_MASK;
        } else if (symbol == 'Q') {
            castleRights |= WHITE_QUEEN_SIDE_CASTLE_MASK;
        } else if (symbol == 'k') {
            castleRights |= BLACK_KING_SIDE_CASTLE_MASK;
        } else if (symbol == 'q') {
            castleRights |= BLACK_QUEEN_SIDE_CASTLE_MASK;
        }
        castleIndex++;
    }
    int enpassantFenIndex = castleIndex + 1;
    if (fenString[enpassantFenIndex] != '-') {
        int squareFile = fenString[enpassantFenIndex] - 'a';
        std::cout << "ENPASSANT ROW :=" << squareFile << "\n";
        enpassantFenIndex++;
        int squareRank = fenString[enpassantFenIndex] - '1';
        std::cout << "ENPASSANT col :=" << squareRank << "\n";
        enPassantSquare = squareRank * BOARD_WIDTH + squareFile;
    }
}

void Board::toggle_side() {
    side == WHITE ? side = BLACK : side = WHITE;
}

void Board::print_board() const {
    std::cout << "\n";

    for (int rank = 7; rank >= 0; rank--) {
        std::cout << rank + 1 << "  ";
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            char pieceChar = '.';
            U64 squareMask = 1ULL << square;
            if (bitboards[WHITE][PAWN] & squareMask) pieceChar = 'P';
            else if (bitboards[WHITE][KNIGHT] & squareMask) pieceChar = 'N';
            else if (bitboards[WHITE][BISHOP] & squareMask) pieceChar = 'B';
            else if (bitboards[WHITE][ROOK] & squareMask) pieceChar = 'R';
            else if (bitboards[WHITE][QUEEN] & squareMask) pieceChar = 'Q';
            else if (bitboards[WHITE][KING] & squareMask) pieceChar = 'K';

            else if (bitboards[BLACK][PAWN] & squareMask) pieceChar = 'p';
            else if (bitboards[BLACK][KNIGHT] & squareMask) pieceChar = 'n';
            else if (bitboards[BLACK][BISHOP] & squareMask) pieceChar = 'b';
            else if (bitboards[BLACK][ROOK] & squareMask) pieceChar = 'r';
            else if (bitboards[BLACK][QUEEN] & squareMask) pieceChar = 'q';
            else if (bitboards[BLACK][KING] & squareMask) pieceChar = 'k';

            std::cout << pieceChar << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n   a b c d e f g h\n\n";
    std::cout << "Side to move: " << (side == WHITE ? "White" : "Black") << "\n";
}

U64 Board::getPieceBitBoard(const Piece piece, const Color color) const {
    if (piece < PAWN || piece > KING || color < WHITE || color > BLACK) {
        return static_cast<U64>(0);
    }
    return bitboards[color][piece];
}

U64 Board::getOccupancies(const Color color) const {
    return occupancies[color];
}

void Board::getAndPrintPawnMovePermissions() {
    std::cout << "permissions mask\n";
    std::cout << std::bitset<16>(doublePawnMoveRight) << "\n";;

    std::cout << "Black Pawn Double Move Permissions" << "\n";
    for (int i = 0; i < boardWidth; i++) {
        if ((doublePawnMoveRight & (static_cast<int16_t>(1) << i))) {
            std::cout << "p" << " ";
        } else {
            std::cout << "." << " ";
        }
    }
    std::cout << "\n White Pawn Double Move Permissions" << "\n";
    for (int i = boardWidth; i < 16; i++) {
        if ((doublePawnMoveRight & (static_cast<int16_t>(1) << i))) {
            std::cout << "P" << " ";
        } else {
            std::cout << "." << " ";
        }
    }
}

uint16_t Board::getDoubleMovePawnPermissions(Color color) const {
    return pawnMasks[color] & doublePawnMoveRight;
}

inline void Board::setEnPassantSquare(const int squareIndex) {
    if (squareIndex > 63 || squareIndex < 0) {
        return;
    };
    enPassantSquare = squareIndex;
}

inline void Board::clearEnPassantSquare() {
    enPassantSquare = -1;
}

int Board::getEnpassantSquare() const {
    return enPassantSquare;
}

void Board::setPieceBitBoard(Color color, Piece piece, U64 newBitBoard) {
    bitboards[color][piece] = newBitBoard;
}

void Board::setOccupanciesBitBoard(Color color, U64 newOccupancy) {
    occupancies[color] = newOccupancy;
}

void Board::removePiece(const int square, Piece piece, Color color) {
    const U64 pieceRemovalBitMask = static_cast<U64>(1) << square;
    bitboards[piece][color] ^= pieceRemovalBitMask;
    occupancies[color] ^= pieceRemovalBitMask;
    occupancies[BOTH] ^= pieceRemovalBitMask;
}

void Board::placePiece(const int square, Piece piece, Color color) {
    const U64 placeMoveBitMask = static_cast<U64>(1) << square;
    bitboards[piece][color] |= placeMoveBitMask;
    occupancies[color] |= placeMoveBitMask;
    occupancies[BOTH] |= placeMoveBitMask;
}

ColorPiece Board::getPieceOnTheIndex(const int index) {
    for (int color = WHITE; color <= BLACK; color++) {
        for (int piece = PAWN; piece <= KING; piece++) {
            U64 pieceBitBoard = bitboards[color][piece];
            U64 checkPositionMask = static_cast<U64>(1) << index;
            if (checkPositionMask & pieceBitBoard) {
                return {
                    static_cast<Color>(color),
                    static_cast<Piece>(piece)
                };
            }
        }
    }
    return {};
}

int Board::getCastleRights(Color color) const {
    assert(color == WHITE || color == BLACK);
    if (color == WHITE) {
        return castleRights & (WHITE_KING_SIDE_CASTLE_MASK | WHITE_QUEEN_SIDE_CASTLE_MASK);
    }

    return castleRights & (BLACK_KING_SIDE_CASTLE_MASK | BLACK_QUEEN_SIDE_CASTLE_MASK);
}

Color Board::getSide() const {
    return side;
}
