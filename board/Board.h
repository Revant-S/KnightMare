//
// Created by revant-sinha on 3/6/26.
//


#ifndef BOARD_H
#define BOARD_H

#include<bits/stdc++.h>
#include "../types_constants/constants.h"
#include "../types_constants/types.h"

class Board {
public:
    Board(const std::string &fenString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    void toggle_side();

    void print_board() const;

    U64 getPieceBitBoard(Piece piece, Color color) const;

    U64 getOccupancies(Color color) const;

    void getAndPrintPawnMovePermissions();

    uint16_t getDoubleMovePawnPermissions(Color color) const;

    void setEnPassantSquare(int squareIndex);

    void clearEnPassantSquare();

    int getEnpassantSquare() const;


    [[nodiscard]] int getCastleRights(Color color) const;

    [[nodiscard]] Color getSide() const;

private:
    U64 bitboards[2][6] = {0};
    U64 occupancies[3] = {
        static_cast<U64>(0),
        static_cast<U64>(0),
        static_cast<U64>(0)
    };
    Color side;
    int boardWidth = ::BOARD_WIDTH;
    int boardHeight = ::BOARD_HEIGHT;
    uint16_t doublePawnMoveRight = 0;

    int castleRights = 0b1111;
    int enPassantSquare = -1;
};
#endif //BOARD_H
