//
// Created by revant-sinha on 3/6/26.
//

#ifndef PREMATCHATTACKCOMPUTATION_H
#define PREMATCHATTACKCOMPUTATION_H

#include "../board/Board.h"

namespace PreMatchAttackComputation {
    inline U64 knightAttacks[64] = {0};
    inline U64 bishopAttacks[64][4] = {0};
    inline U64 rookAttacks[64][4] = {0};
    inline U64 queenAttacks[64][8] = {0};
    inline U64 kingAttacks[64] = {0};
    inline U64 pawnAttacks[2][64] = {0};

    void generateKnightAttacks();
    void generateBishopAttacks();
    void generateRookAttacks();
    void generateQueenAttacks();
    void generateKingAttacks();
    void generatePawnAttacks();
    void init();
} // MoveGeneration

#endif // PREMATCHATTACKCOMPUTATION_H
