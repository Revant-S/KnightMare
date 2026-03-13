//
// Created by revant-sinha on 3/8/26.
//

#include "GenerateLeglMove.h"

#include "../../PreMatchComputations/PreMatchAttackComputation.h"
#include "../../utils/utils.h"

namespace GenerateLegalMove {
    std::vector<Move> getKnightPseudoLegalMoves(Board &board) {
        const Color side = board.getSide();
        U64 friendlyOccupancy = board.getOccupancies(side);
        U64 knightPositions = board.getPieceBitBoard(KNIGHT, side);
        std::vector<Move> knightMoves;
        while (knightPositions) {
            int knightPosition = Utils::getLSB(knightPositions);
            Utils::popLSB(knightPositions);
            U64 pseudoMoves = (PreMatchAttackComputation::knightAttacks[knightPosition] & ~friendlyOccupancy);
            while (pseudoMoves) {
                int destination = Utils::getLSB(pseudoMoves);
                Utils::popLSB(pseudoMoves);
                knightMoves.push_back({
                    knightPosition,
                    destination
                });
            }
        }

        return knightMoves;
    }

    std::vector<Move> getRookPseudoLegalMoves(Board &board) {
        const Color side = board.getSide();
        U64 friendlyOccupancies = board.getOccupancies(side);
        U64 totalOccupancy = board.getOccupancies(BOTH);
        U64 rookPositions = board.getPieceBitBoard(ROOK, side);
        std::vector<Move> rookMoves;
        while (rookPositions) {
            int rookPosition = Utils::getLSB(rookPositions);
            Utils::popLSB(rookPositions);
            for (int direction = NORTH; direction <= WEST; direction++) {
                U64 blockers = PreMatchAttackComputation::rookAttacks[rookPosition][direction] & totalOccupancy;
                U64 finalRay;
                if (blockers) {
                    int nearestBlocker;
                    if (direction == NORTH || direction == EAST) {
                        nearestBlocker = Utils::getLSB(blockers);
                    } else {
                        nearestBlocker = Utils::getMSB(blockers);
                    }
                    U64 subtractRay = PreMatchAttackComputation::rookAttacks[nearestBlocker][direction];
                    U64 fullRay = PreMatchAttackComputation::rookAttacks[rookPosition][direction];
                    finalRay = fullRay ^ subtractRay;
                } else {
                    finalRay = PreMatchAttackComputation::rookAttacks[rookPosition][direction];
                }
                finalRay &= ~friendlyOccupancies;
                while (finalRay) {
                    int destination = Utils::getLSB(finalRay);
                    Utils::popLSB(finalRay);
                    rookMoves.push_back({rookPosition, destination});
                }
            }
        }

        return rookMoves;
    }

    std::vector<Move> getBishopPseudoLegalMoves(Board &board) {
        const Color side = board.getSide();
        U64 friendlyOccupancies = board.getOccupancies(side);
        U64 totalOccupancy = board.getOccupancies(BOTH);
        U64 bishopPositions = board.getPieceBitBoard(BISHOP, side);

        std::vector<Move> bishopMoves;
        while (bishopPositions) {
            int bishopPosition = Utils::getLSB(bishopPositions);
            Utils::popLSB(bishopPositions);
            for (int direction = NORTH_EAST; direction <= SOUTH_WEST; direction++) {
                U64 fullRay = PreMatchAttackComputation::bishopAttacks[bishopPosition][direction - 4];
                U64 blockers = fullRay & totalOccupancy;
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
                    int destination = Utils::getLSB(finalRay);
                    Utils::popLSB(finalRay);
                    bishopMoves.push_back({
                        bishopPosition,
                        destination
                    });
                }
            }
        }
        return bishopMoves;
    }

    std::vector<Move> getQueenPseudoLegalMoves(Board &board) {
        const Color side = board.getSide();
        U64 friendlyOccupancy = board.getOccupancies(side);
        U64 totalOccupancy = board.getOccupancies(BOTH);
        U64 queenPositions = board.getPieceBitBoard(QUEEN, side);
        std::vector<Move> queenMoves;
        while (queenPositions) {
            int queenPosition = Utils::getLSB(queenPositions);
            Utils::popLSB(queenPositions);
            for (int direction = NORTH; direction <= SOUTH_WEST; direction++) {
                U64 fullRay = PreMatchAttackComputation::queenAttacks[queenPosition][direction];
                U64 blockers = fullRay & totalOccupancy;
                U64 finalRay = fullRay;
                if (blockers) {
                    int nearestBlocker;
                    if (direction == NORTH || direction == EAST ||
                        direction == NORTH_EAST || direction == NORTH_WEST) {
                        nearestBlocker = Utils::getLSB(blockers);
                    } else {
                        nearestBlocker = Utils::getMSB(blockers);
                    }

                    U64 subtractRay = PreMatchAttackComputation::queenAttacks[nearestBlocker][direction];
                    finalRay = fullRay ^ subtractRay;
                }
                finalRay &= ~friendlyOccupancy;
                while (finalRay) {
                    int destination = Utils::getLSB(finalRay);
                    Utils::popLSB(finalRay);
                    queenMoves.push_back({
                        queenPosition,
                        destination
                    });
                }
            }
        }
        return queenMoves;
    }

    std::vector<Move> getKingPseudoLegalMoves(Board &board) {
        const Color side = board.getSide();
        U64 friendlyOccupancy = board.getOccupancies(side);
        U64 kingPositions = board.getPieceBitBoard(KING, side);
        std::vector<Move> kingMoves;
        while (kingPositions) {
            int kingPosition = Utils::getLSB(kingPositions);
            Utils::popLSB(kingPositions);
            U64 pseudoMoves = PreMatchAttackComputation::kingAttacks[kingPosition] & ~friendlyOccupancy;
            while (pseudoMoves) {
                int destination = Utils::getLSB(pseudoMoves);
                Utils::popLSB(pseudoMoves);
                kingMoves.push_back({
                    kingPosition,
                    destination
                });
            }
        }

        return kingMoves;
    }

    std::vector<Move> getPawnPseudoLegalMoves(Board &board) {
        const Color side = board.getSide();
        const uint16_t doubleMoveRights = board.getDoubleMovePawnPermissions(side);
        const U64 pawnPositions = board.getPieceBitBoard(PAWN, side);

    }
} // GenerateLegalMove
