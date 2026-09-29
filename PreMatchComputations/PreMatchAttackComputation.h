//
// Created by revant-sinha on 3/6/26.
//

#ifndef PREMATCHATTACKCOMPUTATION_H
#define PREMATCHATTACKCOMPUTATION_H

#include "../board/Board.h"

namespace PreMatchAttackComputation {
    struct Magic {
        U64 relevantOccupancy = 0;
        U64 magicNumber = 0;
        int shift = 0;
        U64 *attacks = nullptr;

        [[nodiscard]] U64 attacksFor(const U64 occupancy) const {
            return attacks[((occupancy & relevantOccupancy) * magicNumber) >> shift];
        }
    };

    inline U64 knightAttacks[64] = {0};
    inline U64 bishopRays[64][4] = {0};
    inline U64 rookRays[64][4] = {0};
    inline U64 kingAttacks[64] = {0};
    inline U64 pawnAttacks[2][64] = {0};
    inline U64 squaresBetween[64][64] = {0};
    inline U64 lineThrough[64][64] = {0};
    inline Magic rookMagics[64];
    inline Magic bishopMagics[64];

    inline U64 getRookAttacks(const int square, const U64 occupancy) {
        return rookMagics[square].attacksFor(occupancy);
    }

    inline U64 getBishopAttacks(const int square, const U64 occupancy) {
        return bishopMagics[square].attacksFor(occupancy);
    }

    inline U64 getQueenAttacks(const int square, const U64 occupancy) {
        return getRookAttacks(square, occupancy) | getBishopAttacks(square, occupancy);
    }

    void generateKnightAttacks();
    void generateBishopRays();
    void generateRookRays();
    void generateKingAttacks();
    void generatePawnAttacks();
    void generateMagics();
    void generateLineTables();
    void init();
} // MoveGeneration

#endif // PREMATCHATTACKCOMPUTATION_H
