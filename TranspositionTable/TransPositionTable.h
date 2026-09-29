//
// Created by revant-sinha on 4/28/26.
//

#ifndef TRANSPOSITIONTABLE_H
#define TRANSPOSITIONTABLE_H
#include <cstddef>
#include "../types_constants/types.h"

namespace TranspositionTable {
    enum Bound : uint8_t {
        EXACT, LOWER_BOUND, UPPER_BOUND
    };

    struct Entry {
        U64 key = 0;
        Move bestMove;
        int score = 0;
        int depth = -1;
        Bound bound = EXACT;
    };

    void resize(std::size_t sizeInMegabytes);

    void clear();

    bool probe(U64 key, Entry &entry);

    void store(U64 key, int depth, int score, Bound bound, const Move &bestMove);
} // TranspositionTable

#endif //TRANSPOSITIONTABLE_H
