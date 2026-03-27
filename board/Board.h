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

    void setPieceBitBoard(Color color, Piece piece, U64 newBitBoard);

    void setOccupanciesBitBoard(Color color, U64 newOccupancy);

    void removePiece(int square, Piece piece, Color color);

    void placePiece(int square, Piece piece, Color color);

    void makeMove(Move &move, Color color);

    void handleCaptureForMove(Move &move);

    BoardState saveState();

    void unmakeMove(BoardState savedState);

    void clearALlCastleRightsOf(Color color);

    void clearCastleRight(Color color , Piece piece);

    ColorPiece getPieceOnTheIndex(int index) const;


    [[nodiscard]] int getCastleRights(Color color) const;

    [[nodiscard]] Color getSide() const;

private:
    std::array<std::array<U64, 6>, 2> bitboards = {0};
    std::array<U64, 3> occupancies = {
        static_cast<U64>(0),
        static_cast<U64>(0),
        static_cast<U64>(0)
    };
    Color side;
    int boardWidth = ::BOARD_WIDTH;
    int boardHeight = ::BOARD_HEIGHT;
    uint16_t doublePawnMoveRight = 0;

    int castleRights = 0b1111; // black queen | black king white queen white king
    int enPassantSquare = -1;
    std::array<ColorPiece , 64> mailBox;
};
#endif //BOARD_H
