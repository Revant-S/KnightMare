//
// Created by revant-sinha on 3/17/26.
//

#include "MoveFunctions.h"

#include "../../PreMatchComputations/PreMatchAttackComputation.h"
#include "../../utils/utils.h"

namespace MoveFunctions {
    void handleCaptureForMove(Move &move, Board &board) {
        Color enemy = board.getSide() == WHITE ? BLACK : WHITE;
        U64 enemyOccupancy = board.getOccupancies(enemy);

        if (move.moveType == SIMPLE) {
        }
    }

    bool isKingInCheck(Color color, Board &board) {
        U64 kingBitBoard = board.getPieceBitBoard(KING, color);
        int kingIndex = Utils::getLSB(kingBitBoard);
        const Color enemy = (color == WHITE) ? BLACK : WHITE;
        const U64 totalOccupancy = board.getOccupancies(BOTH);
        const U64 friendlyOccupancy = board.getOccupancies(color);

        // straight directions: enemy ROOK or QUEEN
        for (int direction = NORTH; direction <= WEST; direction++) {
            U64 fullRay = PreMatchAttackComputation::rookAttacks[kingIndex][direction];
            U64 blockerRay = fullRay & totalOccupancy;
            U64 finalRay = fullRay;
            if (blockerRay) {
                int nearestBlocker = (direction == NORTH || direction == EAST)
                                         ? Utils::getLSB(blockerRay)
                                         : Utils::getMSB(blockerRay);
                finalRay = fullRay ^ PreMatchAttackComputation::rookAttacks[nearestBlocker][direction];
            }
            finalRay &= ~friendlyOccupancy;
            while (finalRay) {
                const int pieceIndex = Utils::getLSB(finalRay);
                Utils::popLSB(finalRay);
                const auto [pieceColor, piece] = board.getPieceOnTheIndex(pieceIndex);
                if (pieceColor == enemy && (piece == ROOK || piece == QUEEN))
                    return true;
            }
        }

        // diagonal directions: enemy BISHOP or QUEEN
        for (int direction = NORTH_EAST; direction <= SOUTH_WEST; direction++) {
            U64 fullRay = PreMatchAttackComputation::bishopAttacks[kingIndex][direction - 4];
            U64 blockerRay = fullRay & totalOccupancy;
            U64 finalRay = fullRay;
            if (blockerRay) {
                int nearestBlocker = (direction == NORTH_EAST || direction == NORTH_WEST)
                                         ? Utils::getLSB(blockerRay)
                                         : Utils::getMSB(blockerRay);
                finalRay = fullRay ^ PreMatchAttackComputation::bishopAttacks[nearestBlocker][direction - 4];
            }
            finalRay &= ~friendlyOccupancy;
            while (finalRay) {
                const int pieceIndex = Utils::getLSB(finalRay);
                Utils::popLSB(finalRay);
                const auto [pieceColor, piece] = board.getPieceOnTheIndex(pieceIndex);
                if (pieceColor == enemy && (piece == BISHOP || piece == QUEEN))
                    return true;
            }
        }

        // pretend to be knight
        U64 knightAttacks = PreMatchAttackComputation::knightAttacks[kingIndex];
        while (knightAttacks) {
            const int sq = Utils::getLSB(knightAttacks);
            Utils::popLSB(knightAttacks);
            const auto [pieceColor, piece] = board.getPieceOnTheIndex(sq);
            if (pieceColor == enemy && piece == KNIGHT)
                return true;
        }

        // pretend to be pawn
        // use OUR color's pawn attacks, squares our pawn would attack FROM kingIndex
        // are exactly where an enemy pawn would need to sit to attack us
        U64 pawnAttacks = PreMatchAttackComputation::pawnAttacks[color][kingIndex];
        while (pawnAttacks) {
            const int sq = Utils::getLSB(pawnAttacks);
            Utils::popLSB(pawnAttacks);
            const auto [pieceColor, piece] = board.getPieceOnTheIndex(sq);
            if (pieceColor == enemy && piece == PAWN)
                return true;
        }

        // king attacks (prevents adjacent kings)
        U64 kingAttacks = PreMatchAttackComputation::kingAttacks[kingIndex];
        while (kingAttacks) {
            const int sq = Utils::getLSB(kingAttacks);
            Utils::popLSB(kingAttacks);
            const auto [pieceColor, piece] = board.getPieceOnTheIndex(sq);
            if (pieceColor == enemy && piece == KING)
                return true;
        }

        return false;
    }


    void makeMove(Move &move, Board &board) {
        Color color = board.getSide();
        if (move.moveType == CASTLE_KING_SIDE || move.moveType == CASTLE_QUEEN_SIDE) {
        }
        board.removePiece(move.from, move.piece, color);
        board.placePiece(move.to, move.piece, color);
    }
} // MoveFunctions
