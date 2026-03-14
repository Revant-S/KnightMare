//
// Created by revant-sinha on 3/6/26.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "types.h"

constexpr int BOARD_WIDTH = 8;
constexpr int BOARD_HEIGHT = 8;

/**
 * If Using CLION
 * IGNORE THE X AND Y SHOWN IN THE CLION THAT IS MISLEADING
 */
inline std::vector<std::pair<int, int> > directions = {
    // Rooks
    {1, 0}, // NORTH (Rank +1, File 0)
    {-1, 0}, // SOUTH (Rank -1, File 0)
    {0, 1}, // EAST  (Rank 0, File +1)
    {0, -1}, // WEST  (Rank 0, File -1)
    // Bishops
    {1, 1}, // NORTH_EAST (Rank +1, File +1)
    {1, -1}, // NORTH_WEST (Rank +1, File -1)
    {-1, 1}, // SOUTH_EAST (Rank -1, File +1)
    {-1, -1} // SOUTH_WEST (Rank -1, File -1)
};

inline std::vector<std::pair<int, int> > knight_directions = {
    {2, -1}, {2, 1}, {-2, 1}, {-2, -1},
    {1, -2}, {-1, -2}, {1, 2}, {-1, 2}
};


inline std::map<char, ColorPiece> fenEnumMapping = {
    // White Pieces (Uppercase)
    {'P', {WHITE, PAWN}},
    {'N', {WHITE, KNIGHT}},
    {'B', {WHITE, BISHOP}},
    {'R', {WHITE, ROOK}},
    {'Q', {WHITE, QUEEN}},
    {'K', {WHITE, KING}},

    // Black Pieces (Lowercase)
    {'p', {BLACK, PAWN}},
    {'n', {BLACK, KNIGHT}},
    {'b', {BLACK, BISHOP}},
    {'r', {BLACK, ROOK}},
    {'q', {BLACK, QUEEN}},
    {'k', {BLACK, KING}}
};

inline uint16_t pawnMasks[2] = {0b1111'1111'0000'0000, 0b0000'0000'1111'1111};


#endif //CONSTANTS_H
