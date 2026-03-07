//
// Created by revant-sinha on 3/6/26.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "types.h"

constexpr int boardWidth = 8;
constexpr int boardHeight = 8;

/**
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

#endif //CONSTANTS_H
