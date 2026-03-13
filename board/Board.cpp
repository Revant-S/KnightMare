//
// Created by revant-sinha on 3/6/26.
//

#include "Board.h"
#include "../types_constants/types.h"
#include <bitset>

Board::Board(std::string fenString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR") {
    side = WHITE;
    U64 whitePawnPositions = static_cast<U64>(0);
    U64 blackPawnPositions = static_cast<U64>(0);
    doublePawnMoveRight = (1 << 16) - 1;
    for (int i = 0; i < boardWidth; i++) {
        int bitPosition = (boardWidth + i);
        const U64 position = static_cast<U64>(1) << bitPosition;
        whitePawnPositions = whitePawnPositions | position;
    }
    for (int i = 0; i < boardWidth; i++) {
        int bitPosition = (boardWidth + i);
        const U64 position = static_cast<U64>(1) << bitPosition;
        blackPawnPositions = blackPawnPositions | position;
    }
    bitboards[WHITE][PAWN] = whitePawnPositions;
    bitboards[BLACK][PAWN] = blackPawnPositions;
    bitboards[WHITE][KNIGHT] = 0x42ULL;
    bitboards[BLACK][KNIGHT] = 0x4200000000000000ULL;
    bitboards[WHITE][ROOK] = 0x81ULL;
    bitboards[BLACK][ROOK] = 0x8100000000000000ULL;
    bitboards[WHITE][BISHOP] = 0x24ULL;
    bitboards[BLACK][BISHOP] = 0x2400000000000000ULL;
    bitboards[WHITE][QUEEN] = 0x08ULL;
    bitboards[BLACK][QUEEN] = 0x0800000000000000ULL;
    bitboards[WHITE][KING] = 0x10ULL;
    bitboards[BLACK][KING] = 0x1000000000000000ULL;

    // set occupied positions;
    for (int i = PAWN; i <= KING; i++) {
        occupancies[WHITE] = occupancies[WHITE] | bitboards[WHITE][i];
        occupancies[BLACK] = occupancies[BLACK] | bitboards[BLACK][i];
    }
    occupancies[BOTH] = occupancies[WHITE] | occupancies[BLACK];
}

void Board::toggle_side() {
    side == WHITE ? side = BLACK : side = WHITE;
}

void Board::print_board() {
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

uint16_t Board::getDoubleMovePawnPermissions(Color color) {
    if (color > BLACK) {
        return -1;
    }
    return pawnMasks[color] & doublePawnMoveRight;
}


Color Board::getSide() const {
    return side;
}
