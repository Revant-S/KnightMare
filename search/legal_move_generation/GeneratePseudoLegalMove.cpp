//
// Created by revant-sinha on 3/8/26.
//

#include "GeneratePseudoLegalMove.h"
#include "../../PreMatchComputations/PreMatchAttackComputation.h"
#include "../../utils/utils.h"

namespace GeneratePseudoLegalMove {
    MoveList getKnightPseudoLegalMoves(Board &board) {
        const Color side = board.getSide();
        const U64 friendlyOccupancy = board.getOccupancies(side);
        U64 knightPositions = board.getPieceBitBoard(KNIGHT, side);
        MoveList knightMoves;
        while (knightPositions) {
            const int knightPosition = Utils::getLSB(knightPositions);
            Utils::popLSB(knightPositions);
            U64 attacks = PreMatchAttackComputation::knightAttacks[knightPosition] & ~friendlyOccupancy;
            while (attacks) {
                const int destination = Utils::getLSB(attacks);
                Utils::popLSB(attacks);
                knightMoves.addMove({
                    knightPosition,
                    destination,
                    side,
                    KNIGHT,
                    SIMPLE
                });
            }
        }

        return knightMoves;
    }

    MoveList getRookPseudoLegalMoves(Board &board) {
        const Color side = board.getSide();
        const U64 friendlyOccupancies = board.getOccupancies(side);
        const U64 totalOccupancy = board.getOccupancies(BOTH);
        U64 rookPositions = board.getPieceBitBoard(ROOK, side);
        MoveList rookMoves;
        while (rookPositions) {
            const int rookPosition = Utils::getLSB(rookPositions);
            Utils::popLSB(rookPositions);
            for (int direction = NORTH; direction <= WEST; direction++) {
                const U64 blockers = PreMatchAttackComputation::rookAttacks[rookPosition][direction] & totalOccupancy;
                U64 finalRay;
                if (blockers) {
                    int nearestBlocker;
                    if (direction == NORTH || direction == EAST) {
                        nearestBlocker = Utils::getLSB(blockers);
                    } else {
                        nearestBlocker = Utils::getMSB(blockers);
                    }
                    const U64 subtractRay = PreMatchAttackComputation::rookAttacks[nearestBlocker][direction];
                    const U64 fullRay = PreMatchAttackComputation::rookAttacks[rookPosition][direction];
                    finalRay = fullRay ^ subtractRay;
                } else {
                    finalRay = PreMatchAttackComputation::rookAttacks[rookPosition][direction];
                }
                finalRay &= ~friendlyOccupancies;
                while (finalRay) {
                    const int destination = Utils::getLSB(finalRay);
                    Utils::popLSB(finalRay);
                    rookMoves.addMove({
                        rookPosition,
                        destination,
                        side,
                        ROOK
                    });
                }
            }
        }

        return rookMoves;
    }

    MoveList getBishopPseudoLegalMoves(Board &board) {
        const Color side = board.getSide();
        const U64 friendlyOccupancies = board.getOccupancies(side);
        const U64 totalOccupancy = board.getOccupancies(BOTH);
        U64 bishopPositions = board.getPieceBitBoard(BISHOP, side);

        MoveList bishopMoves;
        while (bishopPositions) {
            const int bishopPosition = Utils::getLSB(bishopPositions);
            Utils::popLSB(bishopPositions);
            for (int direction = NORTH_EAST; direction <= SOUTH_WEST; direction++) {
                const U64 fullRay = PreMatchAttackComputation::bishopAttacks[bishopPosition][direction - 4];
                const U64 blockers = fullRay & totalOccupancy;
                U64 finalRay;
                if (blockers) {
                    int nearestBlocker;
                    U64 subtractRay;
                    if (direction == NORTH_EAST || direction == NORTH_WEST) {
                        nearestBlocker = Utils::getLSB(blockers);
                        subtractRay = PreMatchAttackComputation::bishopAttacks[nearestBlocker][direction - 4];
                    } else {
                        nearestBlocker = Utils::getMSB(blockers);
                        subtractRay = PreMatchAttackComputation::bishopAttacks[nearestBlocker][direction - 4];
                    }
                    finalRay = fullRay ^ subtractRay;
                } else {
                    finalRay = fullRay;
                }
                finalRay &= ~friendlyOccupancies;
                while (finalRay) {
                    const int destination = Utils::getLSB(finalRay);
                    Utils::popLSB(finalRay);
                    bishopMoves.addMove({
                        bishopPosition,
                        destination,
                        side,
                        BISHOP
                    });
                }
            }
        }
        return bishopMoves;
    }

    MoveList getQueenPseudoLegalMoves(Board &board) {
        const Color side = board.getSide();
        const U64 friendlyOccupancy = board.getOccupancies(side);
        const U64 totalOccupancy = board.getOccupancies(BOTH);
        U64 queenPositions = board.getPieceBitBoard(QUEEN, side);
        MoveList queenMoves;
        while (queenPositions) {
            const int queenPosition = Utils::getLSB(queenPositions);
            Utils::popLSB(queenPositions);
            for (int direction = NORTH; direction <= SOUTH_WEST; direction++) {
                const U64 fullRay = PreMatchAttackComputation::queenAttacks[queenPosition][direction];
                const U64 blockers = fullRay & totalOccupancy;
                U64 finalRay = fullRay;
                if (blockers) {
                    int nearestBlocker;
                    if (direction == NORTH || direction == EAST ||
                        direction == NORTH_EAST || direction == NORTH_WEST) {
                        nearestBlocker = Utils::getLSB(blockers);
                    } else {
                        nearestBlocker = Utils::getMSB(blockers);
                    }

                    const U64 subtractRay = PreMatchAttackComputation::queenAttacks[nearestBlocker][direction];
                    finalRay = fullRay ^ subtractRay;
                }
                finalRay &= ~friendlyOccupancy;
                while (finalRay) {
                    const int destination = Utils::getLSB(finalRay);
                    Utils::popLSB(finalRay);
                    queenMoves.addMove({
                        queenPosition,
                        destination,
                        side,
                        QUEEN
                    });
                }
            }
        }
        return queenMoves;
    }

    MoveList getKingPseudoLegalMoves(Board &board) {
        const Color side = board.getSide();
        const U64 friendlyOccupancy = board.getOccupancies(side);
        const U64 totalOccupancy = board.getOccupancies(BOTH);
        U64 kingPositions = board.getPieceBitBoard(KING, side);
        const int castleRights = board.getCastleRights(side);

        MoveList kingMoves;

        while (kingPositions) {
            const int kingPosition = Utils::getLSB(kingPositions);
            Utils::popLSB(kingPositions);
            U64 pseudoMoves = PreMatchAttackComputation::kingAttacks[kingPosition] & ~friendlyOccupancy;
            while (pseudoMoves) {
                const int destination = Utils::getLSB(pseudoMoves);
                Utils::popLSB(pseudoMoves);
                kingMoves.addMove({kingPosition, destination, side, KING});
            }
            if (side == WHITE) {
                if ((castleRights & WHITE_KING_SIDE_CASTLE_MASK) &&
                    !(totalOccupancy & WHITE_KING_SIDE_CASTLE_EMPTY)) {
                    kingMoves.addMove({
                        WHITE_KING_SQUARE,
                        WHITE_KING_KING_SIDE_CASTLE_DESTINATION,
                        side,
                        KING,
                        CASTLE_KING_SIDE
                    });
                }
                if ((castleRights & WHITE_QUEEN_SIDE_CASTLE_MASK) &&
                    !(totalOccupancy & WHITE_QUEEN_SIDE_CASTLE_EMPTY)) {
                    kingMoves.addMove({
                        WHITE_KING_SQUARE,
                        WHITE_KING_QUEEN_SIDE_CASTLE_DESTINATION,
                        side,
                        KING,
                        CASTLE_QUEEN_SIDE
                    });
                }
            } else {
                if ((castleRights & BLACK_KING_SIDE_CASTLE_MASK) &&
                    !(totalOccupancy & BLACK_KING_SIDE_CASTLE_EMPTY)) {
                    kingMoves.addMove({
                        BLACK_KING_SQUARE,
                        BLACK_KING_KING_SIDE_CASTLE_DESTINATION,
                        side,
                        KING,
                        CASTLE_KING_SIDE
                    });
                }
                if ((castleRights & BLACK_QUEEN_SIDE_CASTLE_MASK) &&
                    !(totalOccupancy & BLACK_QUEEN_SIDE_CASTLE_EMPTY)) {
                    kingMoves.addMove({
                        BLACK_KING_SQUARE,
                        BLACK_KING_QUEEN_SIDE_CASTLE_DESTINATION,
                        side,
                        KING,
                        CASTLE_QUEEN_SIDE
                    });
                }
            }
        }

        return kingMoves;
    }
    MoveList getPawnPseudoLegalMoves(Board &board) {
        const Color side = board.getSide();
        const Color enemySide = (side == WHITE) ? BLACK : WHITE;

        U64 enemyOccupancies = board.getOccupancies(enemySide);
        const int enpassantSquare = board.getEnpassantSquare();
        if (enpassantSquare != -1) {
            enemyOccupancies |= 1ULL << enpassantSquare;
        }
        const U64 totalOccupancies = board.getOccupancies(BOTH);
        U64 pawnPositions = board.getPieceBitBoard(PAWN, side);
        const int pawnForwardDisplacement = (side == WHITE) ? BOARD_WIDTH : -BOARD_WIDTH;
        MoveList moves;
        while (pawnPositions) {
            const int pawnPosition = Utils::getLSB(pawnPositions);
            Utils::popLSB(pawnPositions);
            U64 pawnAttacks = PreMatchAttackComputation::pawnAttacks[side][pawnPosition] & enemyOccupancies;
            while (pawnAttacks) {
                const int destination = Utils::getLSB(pawnAttacks);
                Utils::popLSB(pawnAttacks);
                if (destination == enpassantSquare) {
                    moves.addMove({
                        pawnPosition,
                        destination,
                        side,
                        PAWN,
                        EN_PASSANT
                    });
                } else if (Utils::checkPawnPromotion(side, destination)) {
                    Utils::populatePromotionMoves(pawnPosition, destination, moves, side);
                } else {
                    moves.addMove({pawnPosition, destination, side, PAWN});
                }
            }
            if (const int oneStepForward = pawnPosition + pawnForwardDisplacement; Utils::checkIndexBounds(
                oneStepForward)) {
                if (const U64 oneStepBit = (static_cast<U64>(1) << oneStepForward); !(oneStepBit & totalOccupancies)) {
                    if (Utils::checkPawnPromotion(side, oneStepForward)) {
                        Utils::populatePromotionMoves(pawnPosition, oneStepForward, moves, side);
                    } else {
                        moves.addMove({pawnPosition, oneStepForward, side, PAWN});
                    }
                    if (Utils::checkDoublePawnMoves(pawnPosition, side)) {
                        if (const int twoStepForwardSquare = pawnPosition + 2 * pawnForwardDisplacement;
                            Utils::checkIndexBounds(twoStepForwardSquare)) {
                            if (const U64 twoStepBit = (static_cast<U64>(1) << twoStepForwardSquare); !(
                                twoStepBit & totalOccupancies)) {
                                moves.addMove({
                                    pawnPosition,
                                    twoStepForwardSquare,
                                    side,
                                    PAWN,
                                    DOUBLE_PAWN_MOVE
                                });
                            }
                        }
                    }
                }
            }
        }

        return moves;
    }
} // GenerateLegalMove
