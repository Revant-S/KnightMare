//
// Created by revant-sinha on 3/6/26.
//
#include "PreMatchAttackComputation.h"
#include <bits/stdc++.h>
#include "../utils/utils.h"

namespace PreMatchAttackComputation {
    void generateKnightAttacks() {
        for (int i = 0; i < 64; i++) {
            U64 possibleMove = static_cast<U64>(0);
            auto [rank, file] = Utils::getCoordinates(i);
            for (auto &it: knight_directions) {
                int nextRank = it.first + rank;
                int nextFile = it.second + file;
                if (nextRank >= 0 && nextFile >= 0 && nextRank < BOARD_HEIGHT && nextFile < BOARD_WIDTH) {
                    possibleMove |= (static_cast<U64>(1) << (nextRank * BOARD_WIDTH + nextFile));
                }
            }
            knightAttacks[i] = (possibleMove);
        }
    }

    void generateBishopAttacks() {
        for (int square = 0; square < 64; square++) {
            auto [rank, file] = Utils::getCoordinates(square);

            for (int direction = NORTH_EAST; direction <= SOUTH_WEST; direction++) {
                int nextRank = rank + directions[direction].first;
                int nextFile = file + directions[direction].second;

                while (nextRank >= 0 && nextFile >= 0 && nextRank < BOARD_HEIGHT && nextFile < BOARD_WIDTH) {
                    // west subtracted as bishop has only diagonal directions so 4 is subtracted
                    bishopAttacks[square][direction - 4] |= static_cast<U64>(1) << (nextRank * BOARD_WIDTH + nextFile);
                    nextRank += directions[direction].first;
                    nextFile += directions[direction].second;
                }
            }
        }
    }

    void generateRookAttacks() {
        for (int i = 0; i < 64; i++) {
            auto [rank, file] = Utils::getCoordinates(i);

            for (int direction = NORTH; direction <= WEST; direction++) {
                int nextRank = rank + directions[direction].first;
                int nextFile = file + directions[direction].second;
                while (nextRank >= 0 && nextFile >= 0 && nextRank < BOARD_HEIGHT && nextFile < BOARD_WIDTH) {
                    rookAttacks[i][direction] |= (static_cast<U64>(1) << (nextRank * BOARD_WIDTH + nextFile));
                    nextRank += directions[direction].first;
                    nextFile += directions[direction].second;
                }
            }
        }
    }

    void generateQueenAttacks() {
        for (int square = 0; square < 64; square++) {
            for (int direction = NORTH; direction <= WEST; direction++) {
                queenAttacks[square][direction] = rookAttacks[square][direction];
            }

            for (int direction = NORTH_EAST; direction <= SOUTH_WEST; direction++) {
                queenAttacks[square][direction] = bishopAttacks[square][direction - 4];
            }
        }
    }

    void generateKingAttacks() {
        for (int square = 0; square < 64; square++) {
            U64 possibleMove = static_cast<U64>(0);
            auto [rank, file] = Utils::getCoordinates(square);
            for (int direction = NORTH; direction <= SOUTH_WEST; direction++) {
                int nextRank = directions[direction].first + rank;
                int nextFile = directions[direction].second + file;
                if (nextRank >= 0 && nextFile >= 0 && nextRank < BOARD_HEIGHT && nextFile < BOARD_WIDTH) {
                    possibleMove |= (static_cast<U64>(1) << (nextRank * BOARD_WIDTH + nextFile));
                }
            }
            kingAttacks[square] = possibleMove;
        }
    }

    // right now the  forward motion of pawn is also classified as Attacks
    void generatePawnAttacks() {
        for (int square = 0; square < 64; square++) {
            U64 whiteAttacks = static_cast<U64>(0);
            U64 blackAttacks = static_cast<U64>(0);
            auto [rank, file] = Utils::getCoordinates(square);

            // Calculate White Pawn Attacks
            for (int direction = NORTH_EAST; direction <= NORTH_WEST; direction++) {
                const int nextRank = rank + directions[direction].first;
                const int nextFile = file + directions[direction].second;
                if (nextRank >= 0 && nextRank < BOARD_HEIGHT && nextFile >= 0 && nextFile < BOARD_WIDTH) {
                    whiteAttacks |= (static_cast<U64>(1) << (nextRank * BOARD_WIDTH + nextFile));
                }
            }

            pawnAttacks[WHITE][square] = whiteAttacks;

            // Calculate Black Pawn Attacks
            for (int direction = SOUTH_EAST; direction <= SOUTH_WEST; direction++) {
                const int nextRank = rank + directions[direction].first;
                const int nextFile = file + directions[direction].second;
                if (nextRank >= 0 && nextRank < BOARD_HEIGHT && nextFile >= 0 && nextFile < BOARD_WIDTH) {
                    blackAttacks |= (static_cast<U64>(1) << (nextRank * BOARD_WIDTH + nextFile));
                }
            }
            pawnAttacks[BLACK][square] = blackAttacks;
        }
    }

    void init() {
        generateKnightAttacks();
        generateBishopAttacks();
        generateRookAttacks();
        generateQueenAttacks();
        generateKingAttacks();
        generatePawnAttacks();
    }
} // MoveGen
