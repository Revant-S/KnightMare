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
        const U64 friendlyOccupancy = board.getOccupancies(color);

        // rook/queen check: cast rays in straight directions from squareIndex
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
            finalRay &= ~friendlyOccupancy;
            while (finalRay) {
                const int pieceIndex = Utils::getLSB(finalRay);
                Utils::popLSB(finalRay);
                const auto [pieceColor, piece] = board.getPieceOnTheIndex(pieceIndex);
                if (pieceColor == enemy && (piece == ROOK || piece == QUEEN))
                    return true;
            }
        }

        // bishop/queen check: cast rays in diagonal directions from squareIndex
        for (int direction = NORTH_EAST; direction <= SOUTH_WEST; direction++) {
            const U64 fullRay = PreMatchAttackComputation::bishopAttacks[squareIndex][direction - 4];
            const U64 blockerRay = fullRay & totalOccupancy;
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

        // knight check: see if any enemy knight can jump to squareIndex
        U64 knightAttacks = PreMatchAttackComputation::knightAttacks[squareIndex];
        while (knightAttacks) {
            const int sq = Utils::getLSB(knightAttacks);
            Utils::popLSB(knightAttacks);
            const auto [pieceColor, piece] = board.getPieceOnTheIndex(sq);
            if (pieceColor == enemy && piece == KNIGHT)
                return true;
        }

        // pawn check: use our color's pawn attack mask from sq uareIndex —
        // those are exactly the squares an enemy pawn would attack us from
        U64 pawnAttacks = PreMatchAttackComputation::pawnAttacks[color][squareIndex];
        while (pawnAttacks) {
            const int sq = Utils::getLSB(pawnAttacks);
            Utils::popLSB(pawnAttacks);
            const auto [pieceColor, piece] = board.getPieceOnTheIndex(sq);
            if (pieceColor == enemy && piece == PAWN)
                return true;
        }

        // king check : if attacked by king at that position
        U64 kingAttacks = PreMatchAttackComputation::kingAttacks[squareIndex];
        while (kingAttacks) {
            const int sq = Utils::getLSB(kingAttacks);
            Utils::popLSB(kingAttacks);
            const auto [pieceColor, piece] = board.getPieceOnTheIndex(sq);
            if (pieceColor == enemy && piece == KING)
                return true;
        }

        return false;
    }

    std::vector<Move> getAllLegalMoves(Board &board) {
        auto allPseudolegalMoves = GeneratePseudoLegalMove::getAllPseudoLegalMoves(board);
        std::vector<Move> allLegalMoves = {};
        // int count = 0;
        for (auto &pieceMoves: allPseudolegalMoves) {
            for (Move &move: pieceMoves) {
                bool legal = LegalMoveFilter::isMoveLegal(board, move);
                // if (!legal && board.getSide() == BLACK) {
                //     std::cout << "REJECTED: " << Utils::moveToString(move)
                //               << " piece=" << move.piece << "\n";
                // }
                if (legal) {
                    allLegalMoves.push_back(move);
                }
            }
        }


        return allLegalMoves;
    }
} // MoveFunctions
