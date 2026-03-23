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
    SIMPLE, PROMOTION, CASTLE_KING_SIDE, CASTLE_QUEEN_SIDE, EN_PASSANT, DOUBLE_PAWN_MOVE
};

struct Move {
    int from = -1;
    int to = -1;
    Color colorOfPieceToMove;
    Piece piece;
    MoveType moveType = SIMPLE;
    Piece promoteTo = PAWN;
    bool isCapture = false;
};

struct ColorPiece {
    Color color;
    Piece piece;
};

struct BoardState {
    std::array<std::array<U64, 6>, 2> bitboards;
    std::array<U64, 3> occupancies;
    int enPassantSquare;
    int castleRights;
    Color side;
};

#endif //TYPES_H
