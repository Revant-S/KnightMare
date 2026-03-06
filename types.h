//
// Created by revant-sinha on 3/6/26.
//
#include <bits/stdc++.h>

#ifndef TYPES_H
#define TYPES_H

typedef uint64_t U64;

struct SquareCoords {
    int rank;
    int file;
};

enum Color {
    WHITE, BLACK, BOTH
};

enum Piece {
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING
};

#endif //TYPES_H
