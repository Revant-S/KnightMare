//
// Created by revant-sinha on 3/6/26.
//

#ifndef MOVEGENERATION_H
#define MOVEGENERATION_H

#include "board.h"

namespace MoveGeneration {
    // precompute attacks
    inline U64 knightAttacks[64] = {0};
    inline U64 bishopAttacks[64] = {0};
    inline U64 rookAttacks[64] = {0};
    inline U64 queenAttacks[64] = {0};
    inline U64 kingAttacks[64] = {0};
    inline U64 pawnAttacks[2][64] = {0};

    void generateKnightAttacks();
    void generateBishopAttacks();
    void generateRookAttacks();
    void generateQueenAttacks();
    void generateKingAttacks();
    void generatePawnAttacks();
    void init();

    U64 getLegalKnightMoves(U64 knightBitBoard, U64 friendlyPieces);
    U64 getLegalBishopMoves(U64 bishopBitBoard, U64 friendlyPieces);

} // MoveGeneration

#endif // MOVEGENERATION_H