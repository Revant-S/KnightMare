//
// Created by revant-sinha on 3/16/26.
//

#include "LegalMoveFilter.h"

#include "MoveFunctions.h"
#include "../../utils/utils.h"


namespace LegalMoveFilter {
    bool canKingCastle(Board &board, Move &move, Color color) {
        const MoveType moveType = move.moveType;
        U64 positionsToCheck = (color == WHITE
                                    ? (moveType == CASTLE_KING_SIDE
                                           ? WHITE_KING_SIDE_CASTLE_EMPTY
                                           : WHITE_QUEEN_SIDE_CASTLE_EMPTY)
                                    : (moveType == CASTLE_KING_SIDE
                                           ? BLACK_KING_SIDE_CASTLE_EMPTY
                                           : BLACK_QUEEN_SIDE_CASTLE_EMPTY
                                    ));
        while (positionsToCheck) {
            const int position = Utils::getLSB(positionsToCheck);
            Utils::popLSB(positionsToCheck);
            if (MoveFunctions::isSquareAttackedByEnemy(color, position, board)) {
                return false;
            }
        }
        return true;
    }


    // std::vector<Move> filterIllegalMoves(Board &board, std::vector<Move> &moves) {
    //     std::vector<Move> legalMoves;
    //     Color side = board.getSide();
    //     for (auto &move: moves) {
    //         if (move.moveType == CASTLE_KING_SIDE || move.moveType == CASTLE_QUEEN_SIDE) {
    //             if (!canKingCastle(board, move, side)) continue;
    //         }
    //         BoardState saved = board.saveState();
    //         board.makeMove(move, board.getSide());
    //         if (!MoveFunctions::isKingInCheck(board.getSide(), board))
    //             legalMoves.push_back(move);
    //         board.unmakeMove(saved);
    //     }
    //     return legalMoves;
    // }

    bool isMoveLegal(Board &board, Move &move) {
        Color side = board.getSide();
        if (move.moveType == CASTLE_KING_SIDE || move.moveType == CASTLE_QUEEN_SIDE) {
            return canKingCastle(board, move, side);
        }
        BoardState saved = board.saveState();
        board.makeMove(move, board.getSide());
        bool isLegal = false;
        if (!MoveFunctions::isKingInCheck(board.getSide(), board))
            isLegal = true;
        board.unmakeMove(saved);
        return isLegal;
    }
} // LegalMoveFilter
