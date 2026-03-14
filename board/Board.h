//
// Created by revant-sinha on 3/6/26.
//

#include<bits/stdc++.h>
#include "../types_constants/constants.h"
#include "../types_constants/types.h"
#ifndef BOARD_H
#define BOARD_H


class Board {
public:
    Board(std::string fenString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    void toggle_side();

    void print_board() const;

    U64 getPieceBitBoard(Piece piece, Color color) const;

    U64 getOccupancies(Color color) const;

    void getAndPrintPawnMovePermissions();

    uint16_t getDoubleMovePawnPermissions(Color color);

    CastleRights getCastleRights(Color color);

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
    CastleRights castleRight[2][2] = {
        {true, true},
        {true, true}
    };
};
#endif //BOARD_H
