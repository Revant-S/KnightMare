//
// Created by revant-sinha on 3/6/26.
//

#include<bits/stdc++.h>

#include "board.h"


#ifndef MOVEGEN_H
#define MOVEGEN_H

namespace MoveGen {
    // precompute attacks
    inline U64 knightAttacks[64] = {0};
    inline U64 bishopAttacks[64] = {0};
    inline U64 rookAttacks[64] = {0};
    inline U64 queenAttacks[64] = {0};
    inline U64 kingAttacks[64] = {0};

    void generateKnightAttacks();
    void generateBishopAttacks();
    void generateRookAttacks();
    void generateQueenAttacks();
    void generateKingAttacks();
} // MoveGen

#endif //MOVEGEN_H
