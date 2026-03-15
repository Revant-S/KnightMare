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

    std::vector<Move> getPawnPseudoLegalMoves(const Board &board) {
        const Color side = board.getSide();
        const Color enemySide = (side == WHITE) ? BLACK : WHITE;

        const U64 enemyOccupancies = board.getOccupancies(enemySide);
        const U64 totalOccupancies = board.getOccupancies(BOTH);
        U64 pawnPositions = board.getPieceBitBoard(PAWN, side);

        std::vector<Move> moves;

        while (pawnPositions) {
            const int pawnPosition = Utils::getLSB(pawnPositions);
            Utils::popLSB(pawnPositions);

            U64 pawnAttacks = PreMatchAttackComputation::pawnAttacks[side][pawnPosition];
            pawnAttacks &= enemyOccupancies;

            while (pawnAttacks) {
                const int destination = Utils::getLSB(pawnAttacks);
                Utils::popLSB(pawnAttacks);
                if (side == WHITE) {
                    if (destination >= 56) {
                        for (int promoteTo = KNIGHT; promoteTo <= QUEEN; promoteTo++) {
                            moves.push_back({
                                pawnPosition,
                                destination,
                                PROMOTION,
                                static_cast<Piece>(promoteTo)
                            });
                        }
                    } else {
                        moves.push_back({pawnPosition, destination});
                    }
                } else {
                    if (destination <= 7) {
                        for (int promoteTo = KNIGHT; promoteTo <= QUEEN; promoteTo++) {
                            moves.push_back({
                                pawnPosition,
                                destination,
                                PROMOTION,
                                static_cast<Piece>(promoteTo)
                            });
                        }
                    } else {
                        moves.push_back({pawnPosition, destination});
                    }
                }
            }

            if (side == WHITE) {
                const int step1 = pawnPosition + BOARD_WIDTH;
                U64 nextPosition = (static_cast<U64>(1) << step1) & totalOccupancies;
                // std::cout << "STEP 1 Bhai Kya HO raha hai  " << step1 << "\n";
                // std::cout << "total Occupancy " << std::bitset<64>(totalOccupancies) << "\n";
                // std::cout << "old White Pawn Position " << std::bitset<64>(pawnPosition) << "\n";
                // std::cout << "new White Pawn Position " << std::bitset<64>(nextPosition) << "\n";
                if (step1 == 57) {
                    std::cout << std::bitset<64>(nextPosition) << "\n";
                }
                if (step1 < 64 && !(nextPosition)) {
                    std::cout << step1 << "\n";
                    std::cout << " Promotion is there Bhai PATA NYI " << (step1 >= 56) << "\n";
                    if (step1 >= 56) {
                        for (int promoteTo = KNIGHT; promoteTo <= QUEEN; promoteTo++) {
                            moves.push_back({
                                pawnPosition,
                                step1,
                                PROMOTION,
                                static_cast<Piece>(promoteTo)
                            });
                        }
                    } else {
                        moves.push_back({
                            pawnPosition,
                            step1
                        });
                    }

                    // 2 steps forward
                    if (const int step2 = pawnPosition + 2 * BOARD_WIDTH;
                        pawnPosition / BOARD_WIDTH == 1 && !((static_cast<U64>(1) << step2) & totalOccupancies)) {
                        moves.push_back(
                            {
                                pawnPosition,
                                step2
                            }
                        );
                    }
                }
            } else {
                // 1 step forward
                const int step1 = pawnPosition - BOARD_WIDTH;
                U64 nextPosition = static_cast<U64>(1) << step1;
                if (step1 >= 0 && !nextPosition) {
                    if (step1 <= 7) {
                        for (int promoteTo = KNIGHT; promoteTo <= QUEEN; promoteTo++) {
                            moves.push_back({
                                pawnPosition,
                                step1,
                                PROMOTION,
                                static_cast<Piece>(promoteTo)
                            });
                        }
                    } else {
                        moves.push_back({pawnPosition, step1});
                    }
                    // 2 steps forward
                    if (const int step2 = pawnPosition - 2 * BOARD_WIDTH;
                        pawnPosition / BOARD_WIDTH == 6 && !((static_cast<U64>(1) << step2) & totalOccupancies)) {
                        moves.push_back({pawnPosition, step2});
                    }
                }
            }
        }

        return moves;
    }
} // GenerateLegalMove
