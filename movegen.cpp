//
// Created by revant-sinha on 3/6/26.
//
#include "movegen.h"
#include "types.h"
#include <bits/stdc++.h>
#include "utils.h"

namespace MoveGen {
    void generateKnightAttacks() {
        std::vector<std::pair<int, int> > displacement = {
            {2, -1}, {2, 1}, {-2, 1}, {-2, -1},
            {1, -2}, {-1, -2}, {1, 2}, {-1, 2}
        };

        for (int i = 0; i < 64; i++) {
            U64 possibleMove = static_cast<U64>(0);
            auto coordinates = Utils::getCoordinates(i);
            for (auto &it: displacement) {
                int x = it.first + coordinates.rank;
                int y = it.second + coordinates.file;
                if (x >= 0 && y >= 0 && x < boardWidth && y < boardHeight) {
                    possibleMove |= (static_cast<U64>(1) << (x * boardWidth + y));
                }
            }
            knightAttacks[i] = possibleMove;
        }
    }

    void generateBishopAttacks() {
        std::vector<std::pair<int, int> > directions = {
            {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
        };

        for (int i = 0; i < 64; i++) {
            U64 possibleMove = static_cast<U64>(0);
            auto coordinates = Utils::getCoordinates(i);

            for (auto &dir: directions) {
                int rank = coordinates.rank + dir.first;
                int file = coordinates.file + dir.second;

                while (rank >= 0 && file >= 0 && rank < boardHeight && file < boardWidth) {
                    possibleMove |= (static_cast<U64>(1) << (rank * boardWidth + file));
                    rank += dir.first;
                    file += dir.second;
                }
            }
            bishopAttacks[i] = possibleMove;
        }
    }

    void generateRookAttacks() {
        std::vector<std::pair<int, int> > directions = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}
        };

        for (int i = 0; i < 64; i++) {
            U64 possibleMove = static_cast<U64>(0);
            auto coordinates = Utils::getCoordinates(i);

            for (auto &dir: directions) {
                int rank = coordinates.rank + dir.first;
                int file = coordinates.file + dir.second;

                while (rank >= 0 && file >= 0 && rank < boardHeight && file < boardWidth) {
                    possibleMove |= (static_cast<U64>(1) << (rank * boardWidth + file));
                    rank += dir.first;
                    file += dir.second;
                }
            }
            rookAttacks[i] = possibleMove;
        }
    }

    void generateQueenAttacks() {
        for (int i = 0; i < 64; i++) {
            queenAttacks[i] = rookAttacks[i] | bishopAttacks[i];
        }
    }

    void generateKingAttacks() {
        std::vector<std::pair<int, int> > displacement = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1},
            {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
        };

        for (int i = 0; i < 64; i++) {
            U64 possibleMove = static_cast<U64>(0);
            auto coordinates = Utils::getCoordinates(i);
            for (auto &it: displacement) {
                int x = it.first + coordinates.rank;
                int y = it.second + coordinates.file;
                if (x >= 0 && y >= 0 && x < boardWidth && y < boardHeight) {
                    possibleMove |= (static_cast<U64>(1) << (x * boardWidth + y));
                }
            }
            kingAttacks[i] = possibleMove;
        }
    }
} // MoveGen
