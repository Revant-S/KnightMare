//
// Created by revant-sinha on 3/16/26.
//

#include "LegalMoveFilter.h"

#include "MoveFunctions.h"
#include "../../utils/utils.h"


namespace LegalMoveFilter {
    bool canKingCastle(Board &board, Move &move, Color color) {
        const MoveType moveType = move.moveType;
        U64 emptySquares = (color == WHITE
                                ? (moveType == CASTLE_KING_SIDE
                                       ? WHITE_KING_SIDE_CASTLE_EMPTY
                                       : WHITE_QUEEN_SIDE_CASTLE_EMPTY)
                                : (moveType == CASTLE_KING_SIDE
                                       ? BLACK_KING_SIDE_CASTLE_EMPTY
                                       : BLACK_QUEEN_SIDE_CASTLE_EMPTY));

        if (emptySquares & board.getOccupancies(BOTH)) return false;

        U64 safeSquares = (color == WHITE
                               ? (moveType == CASTLE_KING_SIDE
                                      ? WHITE_KING_SIDE_CASTLE_SAFE
                                      : WHITE_QUEEN_SIDE_CASTLE_SAFE)
                               : (moveType == CASTLE_KING_SIDE
                                      ? BLACK_KING_SIDE_CASTLE_SAFE
                                      : BLACK_QUEEN_SIDE_CASTLE_SAFE));

        while (safeSquares) {
            const int sq = Utils::getLSB(safeSquares);
            Utils::popLSB(safeSquares);
            if (MoveFunctions::isSquareAttackedByEnemy(color, sq, board))
                return false;
        }
        return true;
    }

    bool isMoveLegal(Board &board, Move &move) {
        Color side = board.getSide();
        if (move.moveType == CASTLE_KING_SIDE || move.moveType == CASTLE_QUEEN_SIDE) {
            return canKingCastle(board, move, side);
        }
        const BoardState saved = board.saveState();

        // if (move.piece == KNIGHT && side == BLACK) {
        //     // std::cout << "before makeMove: " << Utils::moveToString(move) << "\n";
        // }

        board.makeMove(move, side);

        if (move.piece == KNIGHT && side == BLACK) {
            // std::cout << "after makeMove, checking king\n";
            U64 kingBB = board.getPieceBitBoard(KING, BLACK);
            // std::cout << "black king bitboard: " << kingBB << "\n"; // if 0, getLSB will assert
        }

        bool isLegal = !MoveFunctions::isKingInCheck(board.getSide(), board);

        // if (move.piece == KNIGHT && side == BLACK) {
        //     std::cout << "isLegal: " << isLegal << "\n";
        // }

        board.unmakeMove(saved);
        return isLegal;
    }
} // LegalMoveFilter
