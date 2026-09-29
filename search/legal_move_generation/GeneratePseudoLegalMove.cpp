//
// Created by revant-sinha on 3/8/26.
//

#include "GeneratePseudoLegalMove.h"
#include "../../PreMatchComputations/PreMatchAttackComputation.h"
#include "../../utils/utils.h"

namespace GeneratePseudoLegalMove {
    namespace {
        void addMovesFromTargets(const int from, U64 targets, const Color side, const Piece piece, MoveList &moves) {
            while (targets) {
                const int destination = Utils::getLSB(targets);
                Utils::popLSB(targets);
                moves.addMove({from, destination, side, piece});
            }
        }

        template<typename AttackFunction>
        void addPieceMoves(Board &board, const Piece piece, MoveList &moves, AttackFunction attacksFrom) {
            const Color side = board.getSide();
            const U64 notFriendly = ~board.getOccupancies(side);
            const U64 occupancy = board.getOccupancies(BOTH);
            U64 pieces = board.getPieceBitBoard(piece, side);
            while (pieces) {
                const int from = Utils::getLSB(pieces);
                Utils::popLSB(pieces);
                addMovesFromTargets(from, attacksFrom(from, occupancy) & notFriendly, side, piece, moves);
            }
        }
    }

    void getKnightPseudoLegalMoves(Board &board, MoveList &moves) {
        addPieceMoves(board, KNIGHT, moves, [](const int square, U64) {
            return PreMatchAttackComputation::knightAttacks[square];
        });
    }

    void getRookPseudoLegalMoves(Board &board, MoveList &moves) {
        addPieceMoves(board, ROOK, moves, PreMatchAttackComputation::getRookAttacks);
    }

    void getBishopPseudoLegalMoves(Board &board, MoveList &moves) {
        addPieceMoves(board, BISHOP, moves, PreMatchAttackComputation::getBishopAttacks);
    }

    void getQueenPseudoLegalMoves(Board &board, MoveList &moves) {
        addPieceMoves(board, QUEEN, moves, PreMatchAttackComputation::getQueenAttacks);
    }

    void getKingPseudoLegalMoves(Board &board, MoveList &kingMoves) {
        const Color side = board.getSide();
        const U64 friendlyOccupancy = board.getOccupancies(side);
        const U64 totalOccupancy = board.getOccupancies(BOTH);
        U64 kingPositions = board.getPieceBitBoard(KING, side);
        const int castleRights = board.getCastleRights(side);

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
    }
    void getPawnPseudoLegalMoves(Board &board, MoveList &moves) {
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
    }
} // GenerateLegalMove
