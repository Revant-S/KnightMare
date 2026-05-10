#include "EvaluationUtils.h"
#include "../evaluation/Evaluation.h"
#include "../../utils/utils.h"
#include <iostream>
#include <iomanip>

namespace EvaluationUtils {
    static const char *pieceNames[] = {"Pawn", "Knight", "Bishop", "Rook", "Queen", "King"};

    static void printRow(const std::string &label, int white, int black) {
        int diff = white - black;
        std::cout << "│  " << std::left << std::setw(10) << label
                << " │ W:" << std::right << std::setw(6) << white
                << " │ B:" << std::setw(6) << black
                << " │ diff:" << std::showpos << std::setw(6) << diff << std::noshowpos << " │\n";
    }

    void printScoreBreakDown(Board &board, Move &move) {
        std::string moveStr = Utils::moveToString(move);

        // ---- Make the move so we score the resulting position ----
        BoardState saved = board.saveState();
        board.makeMove(move);

        std::cout << "\n┌──────────────────────────────────────────────────┐\n";
        std::cout << "│  After move: " << std::left << std::setw(6) << moveStr
                << "  Phase: " << std::setw(12)
                << (Evaluation::isEndGameReached(board) ? "ENDGAME" : "MIDDLEGAME") << "     │\n";
        std::cout << "├────────────┬──────────┬──────────┬───────────────┤\n";
        std::cout << "│  Piece     │  WHITE   │  BLACK   │     DIFF      │\n";
        std::cout << "├────────────┼──────────┼──────────┼───────────────┤\n";

        int totalWhiteMat = 0, totalBlackMat = 0;
        int totalWhitePST = 0, totalBlackPST = 0;

        for (int p = PAWN; p <= KING; p++) {
            Piece piece = static_cast<Piece>(p);

            int wMat = Evaluation::materialValue(board, piece, WHITE);
            int bMat = Evaluation::materialValue(board, piece, BLACK);
            int wPST = Evaluation::calculatePieceDevelopmentScore(board, WHITE, piece);
            int bPST = Evaluation::calculatePieceDevelopmentScore(board, BLACK, piece);

            totalWhiteMat += wMat;
            totalBlackMat += bMat;
            totalWhitePST += wPST;
            totalBlackPST += bPST;

            std::string matLabel = std::string(pieceNames[p]) + "(mat)";
            std::string pstLabel = std::string(pieceNames[p]) + "(pst)";
            printRow(matLabel, wMat, bMat);
            printRow(pstLabel, wPST, bPST);
            std::cout << "├────────────┼──────────┼──────────┼───────────────┤\n";
        }

        int matBalance = totalWhiteMat - totalBlackMat;
        int pstBalance = totalWhitePST - totalBlackPST;
        int absTotal = matBalance + pstBalance;

        // After makeMove, side has flipped — so the side that JUST moved is now the opponent
        Color movedSide = (board.getSide() == WHITE) ? BLACK : WHITE;
        int relTotal = (movedSide == WHITE) ? absTotal : -absTotal;

        std::cout << "│  " << std::left << std::setw(10) << "MAT TOTAL"
                << " │ W:" << std::right << std::setw(6) << totalWhiteMat
                << " │ B:" << std::setw(6) << totalBlackMat
                << " │ bal:" << std::showpos << std::setw(6) << matBalance << std::noshowpos << "  │\n";
        std::cout << "│  " << std::left << std::setw(10) << "PST TOTAL"
                << " │ W:" << std::right << std::setw(6) << totalWhitePST
                << " │ B:" << std::setw(6) << totalBlackPST
                << " │ bal:" << std::showpos << std::setw(6) << pstBalance << std::noshowpos << "  │\n";
        std::cout << "├────────────┴──────────┴──────────┴───────────────┤\n";
        std::cout << "│  Absolute total  : " << std::showpos << std::setw(6) << absTotal << std::noshowpos
                << "  (WHITE - BLACK)            │\n";
        std::cout << "│  Relative total  : " << std::showpos << std::setw(6) << relTotal << std::noshowpos
                << "  [" << (movedSide == WHITE ? "WHITE" : "BLACK") << " POV]              │\n";
        std::cout << "└──────────────────────────────────────────────────┘\n";

        board.unmakeMove(saved);
    }
} // EvaluationUtils
