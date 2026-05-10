//
// Created by revant-sinha on 3/29/26.
//

#include "Evaluation.h"
#include <bit>

#include "../../utils/utils.h"

namespace Evaluation {
    inline constexpr int END_GAME_POINT_THRESHOLD = 5;
    // 5 step
    int materialValue(Board &board, Piece piece, Color side) {
        const U64 bitBoard = board.getPieceBitBoard(piece, side);
        return pieceRelatialue[piece] * std::popcount(bitBoard);
    }

    int totalMaterialValue(Board &board, Color side) {
        int totalValue = 0;
        for (int piece = PAWN; piece <= KING; piece++) {
            totalValue += materialValue(board, static_cast<Piece>(piece), side);
        }
        return totalValue;
    }

    int compareMaterial(Board &board) {
        return totalMaterialValue(board, WHITE) - totalMaterialValue(board, BLACK);
    }

    std::array<int, 64> getPSTArray(Piece piece, Board &board) {
        if (piece == KING && isEndGameReached(board)) {
            return pstTable[KING + 1]; // KING is for normal games
        }
        return pstTable[piece];
    }

    int kingSafetyScore(Board &board, Color side) {

    }

    int calculatePieceDevelopmentScore(Board &board, Color side, Piece piece) {
        U64 pieceBitBoard = board.getPieceBitBoard(piece, side);
        std::array<int, 64> pstTable = getPSTArray(piece, board);
        int score = 0;
        while (pieceBitBoard) {
            const int position = Utils::getLSB(pieceBitBoard);
            Utils::popLSB(pieceBitBoard);
            score += (side == WHITE ? pstTable[position] : pstTable[(position ^ 56)]); // xor with 56 to flip the board
        }

        return score;
    }

    int assesPieceDevelopment(Board &board) {
        int developmentScore = 0;
        for (int piece = PAWN; piece <= KING; piece++) {
            developmentScore += calculatePieceDevelopmentScore(board, WHITE, static_cast<Piece>(piece));
            developmentScore -= calculatePieceDevelopmentScore(board, BLACK, static_cast<Piece>(piece));
        }
        return developmentScore;
    }

    int evaluate(Board &board) {
        const Color side = board.getSide();
        const int materialBalance = compareMaterial(board);
        const int developmentScore = assesPieceDevelopment(board);
        const int absoluteScore = static_cast<int>(0.7 * materialBalance) + developmentScore;
        return (side == WHITE) ? absoluteScore : -absoluteScore;
    }

    bool isEndGameReached(Board &board) {
        const int whiteSideTotalValue = totalMaterialValue(board, WHITE);
        const int blackSideTotalValue = totalMaterialValue(board, BLACK);
        return (whiteSideTotalValue <= END_GAME_POINT_THRESHOLD && blackSideTotalValue <= END_GAME_POINT_THRESHOLD);
    }
} // Evaluation
