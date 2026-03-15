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

enum Direction {
    NORTH, SOUTH, EAST, WEST, NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST
};

enum MoveType {
    SIMPLE, CASTLE, PROMOTION
};

struct Move {
    int from = -1;
    int to = -1;
    MoveType moveType = SIMPLE;
    Piece promotionPiece = PAWN;
};

struct ColorPiece {
    Color color;
    Piece piece;
};

struct CastleRights {
    bool kingSide;
    bool queenSide;
};

#endif //TYPES_H
