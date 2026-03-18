//
// Created by revant-sinha on 3/16/26.
//

#include "LegalMoveFilter.h"

namespace LegalMoveFilter {

    void filterIllegalMoves(Board &board, std::vector<Move> &moves) {
        /*
         * store the current state of board
         * Make the move
         * check legality
         * unmake move
         *
        For all moves:
  → After making the move, is our king attacked? If yes, illegal.

Additionally for castling moves:
  → Is the king currently in check? (cannot castle out of check)
  → Does the king pass through an attacked square?
  (king ending in check is already caught by the general check above)
         */
    }

    // bool isMoveLegal(Board &board, Move &move) {
    //     Board currentBoardState = board;
    //
    // }
} // LegalMoveFilter
