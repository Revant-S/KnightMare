//
// Created by revant-sinha on 3/17/26.
//

#include "MoveFunctions.h"

#include "GeneratePseudoLegalMove.h"
#include "LegalMoveFilter.h"
#include "../../PreMatchComputations/PreMatchAttackComputation.h"
#include "../../utils/utils.h"

namespace MoveFunctions {
    bool isKingInCheck(Color color, Board &board) {
        const U64 kingBitBoard = board.getPieceBitBoard(KING, color);
        const int kingIndex = Utils::getLSB(kingBitBoard);
        return isSquareAttackedByEnemy(color, kingIndex, board);
    }

    bool isSquareAttackedByEnemy(Color color, int squareIndex, Board &board) {
        const Color enemy = (color == WHITE) ? BLACK : WHITE;
        const U64 totalOccupancy = board.getOccupancies(BOTH);
        const U64 enemyKnights = board.getPieceBitBoard(KNIGHT, enemy);
        const U64 enemyBishops = board.getPieceBitBoard(BISHOP, enemy);
        const U64 enemyRooks = board.getPieceBitBoard(ROOK, enemy);
        const U64 enemyQueens = board.getPieceBitBoard(QUEEN, enemy);
        const U64 enemyPawns = board.getPieceBitBoard(PAWN, enemy);
        const U64 enemyKing = board.getPieceBitBoard(KING, enemy);

        if (PreMatchAttackComputation::knightAttacks[squareIndex] & enemyKnights) return true;
        if (PreMatchAttackComputation::pawnAttacks[color][squareIndex] & enemyPawns) return true;
        if (PreMatchAttackComputation::kingAttacks[squareIndex] & enemyKing) return true;

        for (int direction = NORTH; direction <= WEST; direction++) {
            U64 fullRay = PreMatchAttackComputation::rookAttacks[squareIndex][direction];
            U64 blockerRay = fullRay & totalOccupancy;
            U64 finalRay = fullRay;
            if (blockerRay) {
                int nearestBlocker = (direction == NORTH || direction == EAST)
                                         ? Utils::getLSB(blockerRay)
                                         : Utils::getMSB(blockerRay);
                finalRay = fullRay ^ PreMatchAttackComputation::rookAttacks[nearestBlocker][direction];
            }
            if (finalRay & (enemyRooks | enemyQueens)) return true;
        }

        for (int direction = NORTH_EAST; direction <= SOUTH_WEST; direction++) {
            const U64 fullRay = PreMatchAttackComputation::bishopAttacks[squareIndex][direction - 4];
            const U64 blockerRay = fullRay & totalOccupancy;
            U64 finalRay = fullRay;
            if (blockerRay) {
                const int nearestBlocker = (direction == NORTH_EAST || direction == NORTH_WEST)
                                         ? Utils::getLSB(blockerRay)
                                         : Utils::getMSB(blockerRay);
                finalRay = fullRay ^ PreMatchAttackComputation::bishopAttacks[nearestBlocker][direction - 4];
            }
            if (finalRay & (enemyBishops | enemyQueens)) return true;
        }

        return false;
    }

    MoveList getAllLegalMoves(Board &board) {
        MoveList legalMoves;

        auto check = [&](MoveList &pseudoMoves) {
            for (Move &move : pseudoMoves) {
                if (LegalMoveFilter::isMoveLegal(board, move)) {
                    legalMoves.addMove(move);
                }
            }
        };
        auto pawns   = GeneratePseudoLegalMove::getPawnPseudoLegalMoves(board);
        auto knights = GeneratePseudoLegalMove::getKnightPseudoLegalMoves(board);
        auto bishops = GeneratePseudoLegalMove::getBishopPseudoLegalMoves(board);
        auto rooks   = GeneratePseudoLegalMove::getRookPseudoLegalMoves(board);
        auto queens  = GeneratePseudoLegalMove::getQueenPseudoLegalMoves(board);
        auto kings   = GeneratePseudoLegalMove::getKingPseudoLegalMoves(board);

        check(pawns);
        check(knights);
        check(bishops);
        check(rooks);
        check(queens);
        check(kings);

        return legalMoves;
    }
} // MoveFunctions
