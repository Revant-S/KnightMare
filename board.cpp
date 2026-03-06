//
// Created by revant-sinha on 3/6/26.
//

#include "board.h"


board::board() {
    // start with white
    side = WHITE;
    // set pawn positions
    U64 whitePawnPositions = static_cast<U64>(0);
    U64 blackPawnPositions = static_cast<U64>(0);
    for (int i = 0; i < boardWidth; i++) {
        const U64 position = static_cast<U64>(1) << (boardWidth + i);
        whitePawnPositions = whitePawnPositions | position;
    }
    for (int i = 0; i < boardWidth; i++) {
        const U64 position = static_cast<U64>(1) << (boardWidth * 6 + i);
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

void board::toggle_side() {
    side == WHITE ? side = BLACK : side = WHITE;
}

void board::print_board() {
    std::cout << "\n";

    for (int rank = 7; rank >= 0; rank--) {
        std::cout << rank + 1 << "  "; // Print the rank number on the left

        // Loop from File 0 (a) to File 7 (h)
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            char pieceChar = '.'; // Default to an empty square

            // Create a mask for the current square
            U64 squareMask = 1ULL << square;

            // Check White Pieces
            if (bitboards[WHITE][PAWN] & squareMask) pieceChar = 'P';
            else if (bitboards[WHITE][KNIGHT] & squareMask) pieceChar = 'N';
            else if (bitboards[WHITE][BISHOP] & squareMask) pieceChar = 'B';
            else if (bitboards[WHITE][ROOK] & squareMask) pieceChar = 'R';
            else if (bitboards[WHITE][QUEEN] & squareMask) pieceChar = 'Q';
            else if (bitboards[WHITE][KING] & squareMask) pieceChar = 'K';

                // Check Black Pieces
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

    // Print the game states below the board
    std::cout << "Side to move: " << (side == WHITE ? "White" : "Black") << "\n";
}
