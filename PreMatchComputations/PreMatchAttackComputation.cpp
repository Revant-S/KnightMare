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

    void generateBishopRays() {
        for (int square = 0; square < 64; square++) {
            auto [rank, file] = Utils::getCoordinates(square);

            for (int direction = NORTH_EAST; direction <= SOUTH_WEST; direction++) {
                int nextRank = rank + directions[direction].first;
                int nextFile = file + directions[direction].second;

                while (nextRank >= 0 && nextFile >= 0 && nextRank < BOARD_HEIGHT && nextFile < BOARD_WIDTH) {
                    // west subtracted as bishop has only diagonal directions so 4 is subtracted
                    bishopRays[square][direction - 4] |= static_cast<U64>(1) << (nextRank * BOARD_WIDTH + nextFile);
                    nextRank += directions[direction].first;
                    nextFile += directions[direction].second;
                }
            }
        }
    }

    void generateRookRays() {
        for (int i = 0; i < 64; i++) {
            auto [rank, file] = Utils::getCoordinates(i);

            for (int direction = NORTH; direction <= WEST; direction++) {
                int nextRank = rank + directions[direction].first;
                int nextFile = file + directions[direction].second;
                while (nextRank >= 0 && nextFile >= 0 && nextRank < BOARD_HEIGHT && nextFile < BOARD_WIDTH) {
                    rookRays[i][direction] |= (static_cast<U64>(1) << (nextRank * BOARD_WIDTH + nextFile));
                    nextRank += directions[direction].first;
                    nextFile += directions[direction].second;
                }
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

    void generatePawnAttacks() {
        for (int square = 0; square < 64; square++) {
            U64 whiteAttacks = static_cast<U64>(0);
            U64 blackAttacks = static_cast<U64>(0);
            auto [rank, file] = Utils::getCoordinates(square);


            for (int direction = NORTH_EAST; direction <= NORTH_WEST; direction++) {
                const int nextRank = rank + directions[direction].first;
                const int nextFile = file + directions[direction].second;
                if (nextRank >= 0 && nextRank < BOARD_HEIGHT && nextFile >= 0 && nextFile < BOARD_WIDTH) {
                    whiteAttacks |= (static_cast<U64>(1) << (nextRank * BOARD_WIDTH + nextFile));
                }
            }
            pawnAttacks[WHITE][square] = whiteAttacks;

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

    namespace {
        U64 slidingAttacksSlow(const int square, const U64 occupancy, const U64 (&rays)[64][4], const bool isRook) {
            U64 attacks = 0;
            for (int direction = 0; direction < 4; direction++) {
                const U64 fullRay = rays[square][direction];
                const U64 blockers = fullRay & occupancy;
                if (!blockers) {
                    attacks |= fullRay;
                    continue;
                }
                const bool towardsHigherSquares = isRook ? (direction == 0 || direction == 2) : (direction <= 1);
                const int nearestBlocker = towardsHigherSquares ? Utils::getLSB(blockers) : Utils::getMSB(blockers);
                attacks |= fullRay ^ rays[nearestBlocker][direction];
            }
            return attacks;
        }

        U64 edgesNotOnRay(const int square) {
            constexpr U64 RANK_1 = 0xFFULL, RANK_8 = 0xFFULL << 56;
            constexpr U64 FILE_A = FILE_A_MASK, FILE_H = FILE_A_MASK << 7;
            const U64 rank = 0xFFULL << (8 * (square / BOARD_WIDTH));
            const U64 file = FILE_A_MASK << (square % BOARD_WIDTH);
            return ((RANK_1 | RANK_8) & ~rank) | ((FILE_A | FILE_H) & ~file);
        }

        std::vector<U64> rookAttackTable;
        std::vector<U64> bishopAttackTable;

        void findMagics(Magic (&magics)[64], std::vector<U64> &table, const U64 (&rays)[64][4], const bool isRook) {
            std::mt19937_64 generator(isRook ? 0x526F6F6BULL : 0x42697368ULL);
            std::vector<std::size_t> offsets(64);
            std::size_t tableSize = 0;
            for (int square = 0; square < 64; square++) {
                U64 fullRays = 0;
                for (int direction = 0; direction < 4; direction++) fullRays |= rays[square][direction];
                magics[square].relevantOccupancy = fullRays & ~edgesNotOnRay(square);
                magics[square].shift = 64 - std::popcount(magics[square].relevantOccupancy);
                offsets[square] = tableSize;
                tableSize += 1ULL << std::popcount(magics[square].relevantOccupancy);
            }
            table.assign(tableSize, 0);

            std::vector<U64> occupancies, attacks;
            std::vector<int> usedAt;
            for (int square = 0; square < 64; square++) {
                Magic &magic = magics[square];
                magic.attacks = table.data() + offsets[square];
                occupancies.clear();
                attacks.clear();
                U64 subset = 0;
                do {
                    occupancies.push_back(subset);
                    attacks.push_back(slidingAttacksSlow(square, subset, rays, isRook));
                    subset = (subset - magic.relevantOccupancy) & magic.relevantOccupancy;
                } while (subset);

                const std::size_t entries = occupancies.size();
                usedAt.assign(entries, -1);
                for (int attempt = 1;; attempt++) {
                    magic.magicNumber = generator() & generator() & generator();
                    if (std::popcount((magic.relevantOccupancy * magic.magicNumber) >> 56) < 6) continue;
                    bool isValid = true;
                    for (std::size_t index = 0; index < entries && isValid; index++) {
                        const std::size_t slot = (occupancies[index] * magic.magicNumber) >> magic.shift;
                        if (usedAt[slot] != attempt) {
                            usedAt[slot] = attempt;
                            magic.attacks[slot] = attacks[index];
                        } else if (magic.attacks[slot] != attacks[index]) {
                            isValid = false;
                        }
                    }
                    if (isValid) break;
                }
            }
        }
    }

    void generateMagics() {
        findMagics(rookMagics, rookAttackTable, rookRays, true);
        findMagics(bishopMagics, bishopAttackTable, bishopRays, false);
    }

    void generateLineTables() {
        for (int from = 0; from < 64; from++) {
            for (int to = 0; to < 64; to++) {
                if (from == to) continue;
                const U64 fromBit = 1ULL << from, toBit = 1ULL << to;
                if (getRookAttacks(from, 0) & toBit) {
                    squaresBetween[from][to] = getRookAttacks(from, toBit) & getRookAttacks(to, fromBit);
                    lineThrough[from][to] = (getRookAttacks(from, 0) & getRookAttacks(to, 0)) | fromBit | toBit;
                } else if (getBishopAttacks(from, 0) & toBit) {
                    squaresBetween[from][to] = getBishopAttacks(from, toBit) & getBishopAttacks(to, fromBit);
                    lineThrough[from][to] = (getBishopAttacks(from, 0) & getBishopAttacks(to, 0)) | fromBit | toBit;
                }
            }
        }
    }

    void init() {
        generateKnightAttacks();
        generateBishopRays();
        generateRookRays();
        generateKingAttacks();
        generatePawnAttacks();
        generateMagics();
        generateLineTables();
    }
} // MoveGen
