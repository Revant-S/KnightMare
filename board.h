//
// Created by revant-sinha on 3/6/26.
//
#include<bits/stdc++.h>

#include "constants.h"
#include "types.h"
#ifndef BOARD_H
#define BOARD_H


class board {
public:
    board();

    void toggle_side();

    void print_board();

    U64 getPieceBitBoard(Piece piece, Color color) const;

    U64 getOccupancies(Color color);

private:
    U64 bitboards[2][6];
    U64 occupancies[3] = {
        static_cast<U64>(0),
        static_cast<U64>(0),
        static_cast<U64>(0)
    };
    int side;
    int boardWidth = ::boardWidth;
    int boardHeight = ::boardHeight;
};
#endif //BOARD_H
