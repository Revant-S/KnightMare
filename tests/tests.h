//
// Created by revant-sinha on 3/7/26.
//

#ifndef TESTS_H
#define TESTS_H

#include "../board/Board.h"
#include "../types_constants/types.h"


namespace tests {
    void visualizeMoves(const MoveList &moves);

    int perft(Board &board, int depth);

    void perftDivide(Board &board, int depth);

    void runPerftSuite(Board &board, const std::vector<PerftResult> &expectedResults, const std::string &suiteName);

    std::map<std::string, int> getStockfishPerft(const std::string &fen, int depth);

    void isoLateIncorrectMove(Board board, const std::string &fen, int depth);

    void compareWithStockfish(Board &board, const std::string &fen, int depth);

    void testAllPositions();
};


#endif //TESTS_H
