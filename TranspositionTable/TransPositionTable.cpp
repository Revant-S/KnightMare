//
// Created by revant-sinha on 4/28/26.
//

#include "TransPositionTable.h"
#include <vector>

namespace TranspositionTable {
    namespace {
        constexpr std::size_t DEFAULT_SIZE_IN_MEGABYTES = 32;
        std::vector<Entry> table(DEFAULT_SIZE_IN_MEGABYTES * 1024 * 1024 / sizeof(Entry));

        Entry &slotFor(const U64 key) {
            return table[key % table.size()];
        }
    }

    void resize(const std::size_t sizeInMegabytes) {
        table.assign(std::max<std::size_t>(1, sizeInMegabytes * 1024 * 1024 / sizeof(Entry)), Entry{});
    }

    void clear() {
        std::fill(table.begin(), table.end(), Entry{});
    }

    bool probe(const U64 key, Entry &entry) {
        const Entry &slot = slotFor(key);
        if (slot.key != key || slot.depth < 0) return false;
        entry = slot;
        return true;
    }

    void store(const U64 key, const int depth, const int score, const Bound bound, const Move &bestMove) {
        Entry &slot = slotFor(key);
        if (slot.key == key && slot.depth > depth && bound != EXACT) return;
        const bool keepPreviousMove = bestMove.from == -1 && slot.key == key;
        slot = {key, keepPreviousMove ? slot.bestMove : bestMove, score, depth, bound};
    }
} // TranspositionTable
