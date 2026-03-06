//
// Created by revant-sinha on 3/6/26.
//
#include<bits/stdc++.h>

#ifndef BOARD_H
#define BOARD_H

typedef uint64_t U64;

enum Color {
    WHITE, BLACK, BOTH
};

enum Piece {
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING
};

class board {
public:
    board();
    void toggle_side();
    void print_board();

private:
    // boards
    U64 bitboards[2][6];
    U64 occupancies[3] = {
        static_cast<U64>(0),
        static_cast<U64>(0),
        static_cast<U64>(0)
    };
    // game states
    int side;
    // constants
    int boardWidth = 8;
    int boardHeight = 8;
    int typesOfPieces = 6;
};
#endif //BOARD_H
