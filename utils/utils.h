#ifndef UTILS_H
#define UTILS_H
#include "../board/Board.h"
#include "../types_constants/types.h"

namespace Utils {
    void printBitBoard(U64 bitBoard);

    void compareFen(const std::string &fenGenerated, const std::string &fenToCompare);

    SquareCoords getCoordinates(int square);

    std::string squareToString(int square);

    std::string moveToString(const Move &move);

    void printMoves(MoveList &moves);

    void printAllPseudoLegalMoves(Board &board);

    void printCastleRights(Board &board);

    void populatePromotionMoves(int pawnPosition, int destination, MoveList &moves, Color side);

    Move parseMoveString(const std::string &moveStr, Board &board);

    std::string getFenAfterMove(const std::string &fen, const std::string &move);

    inline bool checkIndexBounds(int index) {
        return (index <= FINAL_SQUARE_INDEX && index >= FIRST_SQUARE_INDEX);
    }

    inline bool checkPawnPromotion(Color side, int nextIndex) {
        return (side == WHITE && nextIndex >= BLACK_BACK_RANK_THRESHOLD_INDEX) ||
               (side == BLACK && nextIndex <= WHITE_BACK_RANK_THRESHOLD_INDEX);
    }

    inline bool checkDoublePawnMoves(int index, Color side) {
        return (side == WHITE && index / BOARD_WIDTH == 1) ||
               (side == BLACK && index / BOARD_WIDTH == 6);
    }

    inline int getLSB(const U64 bb) {
        assert(bb != 0);
        return std::countr_zero(bb);
    }

    inline int getMSB(const U64 bb) {
        assert(bb != 0);
        return 63 - __builtin_clzll(bb);
    }

    inline void popLSB(U64 &bb) {
        if (bb) bb &= (bb - 1);
    }
} // Utils

#endif //UTILS_H
