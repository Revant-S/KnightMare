//
// Created by revant-sinha on 3/6/26.
//
#include "MoveGeneration.h"
#include <bits/stdc++.h>
#include "utils.h"

namespace MoveGeneration {
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
            knightAttacks[i] = (possibleMove);
        }
    }

    void generateBishopAttacks() {
        for (int square = 0; square < 64; square++) {
            auto coordinates = Utils::getCoordinates(square);

            for (int direction = NORTH_EAST; direction <= SOUTH_WEST; direction++) {
                int x = coordinates.rank + directions[direction].first;
                int y = coordinates.file + directions[direction].second;

                while (x >= 0 && y >= 0 && x < boardWidth && y < boardHeight) {
                    // west subtracted as bishop has only diagonal directions so 4 is subtracted
                    bishopAttacks[square][direction - 4] |= static_cast<U64>(1) << (x * boardWidth + y);
                    x += directions[direction].first;
                    y += directions[direction].second;
                }
            }
        }
    }

    void generateRookAttacks() {
        for (int i = 0; i < 64; i++) {
            auto coordinates = Utils::getCoordinates(i);

            for (int j = NORTH; j <= WEST; j++) {
                int x = coordinates.rank + directions[j].first;
                int y = coordinates.file + directions[j].second;
                while (x >= 0 && y >= 0 && x < boardWidth && y < boardHeight) {
                    rookAttacks[i][j] |= (static_cast<U64>(1) << (x * boardWidth + y));
                    x += directions[j].first;
                    y += directions[j].second;
                }
            }
        }
    }

    void generateQueenAttacks() {
        for (int i = 0; i < 64; i++) {
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

    void generatePawnAttacks() {
        std::vector<std::pair<int, int> > whiteDisplacements = {
            {1, -1}, {1, 1}
        };
        std::vector<std::pair<int, int> > blackDisplacements = {
            {-1, -1}, {-1, 1}
        };

        for (int i = 0; i < 64; i++) {
            U64 whiteAttacks = static_cast<U64>(0);
            U64 blackAttacks = static_cast<U64>(0);
            auto coordinates = Utils::getCoordinates(i);

            // Calculate White Pawn Attacks
            for (auto &it: whiteDisplacements) {
                int rank = coordinates.rank + it.first;
                int file = coordinates.file + it.second;
                if (rank >= 0 && rank < boardHeight && file >= 0 && file < boardWidth) {
                    whiteAttacks |= (static_cast<U64>(1) << (rank * boardWidth + file));
                }
            }
            pawnAttacks[WHITE][i] = whiteAttacks;

            // Calculate Black Pawn Attacks
            for (auto &it: blackDisplacements) {
                int rank = coordinates.rank + it.first;
                int file = coordinates.file + it.second;
                if (rank >= 0 && rank < boardHeight && file >= 0 && file < boardWidth) {
                    blackAttacks |= (static_cast<U64>(1) << (rank * boardWidth + file));
                }
            }
            pawnAttacks[BLACK][i] = blackAttacks;
        }
    }

    void init() {
        generateKnightAttacks();
        generateBishopAttacks();
        generateRookAttacks();
        generateQueenAttacks();
        generateKingAttacks();
    }

    U64 getLegalKnightMoves(U64 knightBitBoard, U64 friendlyPieces) {
        U64 legalPositions = static_cast<U64>(0);
        while (knightBitBoard) {
            const U64 position = Utils::getLSB(knightBitBoard);
            const U64 attacks = knightAttacks[position];
            legalPositions |= attacks & ~friendlyPieces;
            Utils::popLSB(knightBitBoard);
        }
        return legalPositions;
    }

    // U64 getLegalBishopMoves(U64 bishopBitBoard, U64 friendlyPieces) {
    //
    // }
} // MoveGen
