#include "EvaluationUtils.h"
#include "../evaluation/Evaluation.h"
#include "../../utils/utils.h"
#include <iostream>
#include <iomanip>

namespace EvaluationUtils {
    static void printRow(const std::string &label, const int white, const int black) {
        std::cout << "│  " << std::left << std::setw(14) << label
                << " │ W:" << std::right << std::setw(6) << white
                << " │ B:" << std::setw(6) << black
                << " │ diff:" << std::showpos << std::setw(6) << white - black << std::noshowpos << " │\n";
    }

    void printScoreBreakDown(Board &board, Move &move) {
        using Term = Evaluation::TaperedScore (*)(const Board &, Color);
        const std::pair<const char *, Term> terms[] = {
            {"Material", Evaluation::materialScore},
            {"PieceSquare", Evaluation::pieceSquareScore},
            {"PawnStructure", Evaluation::pawnStructureScore},
            {"Activity", Evaluation::pieceActivityScore},
            {"KingSafety", Evaluation::kingSafetyScore},
        };

        BoardState saved = board.saveState();
        board.makeMove(move);
        board.toggle_side();

        const int phase = Evaluation::gamePhase(board);
        std::cout << "\n┌──────────────────────────────────────────────────────┐\n";
        std::cout << "│  After move: " << std::left << std::setw(6) << Utils::moveToString(move)
                << "  Phase: " << std::setw(2) << phase << "/" << TOTAL_GAME_PHASE << "                     │\n";
        std::cout << "├──────────────────────────────────────────────────────┤\n";

        for (const auto &[name, term]: terms) {
            const int white = Evaluation::blendByPhase(term(board, WHITE), phase);
            const int black = Evaluation::blendByPhase(term(board, BLACK), phase);
            printRow(name, white, black);
        }
        printRow("MopUp", Evaluation::mopUpScore(board), 0);

        std::cout << "├──────────────────────────────────────────────────────┤\n";
        std::cout << "│  Side to move score: " << std::showpos << std::setw(6) << Evaluation::evaluate(board)
                << std::noshowpos << "  [" << (board.getSide() == WHITE ? "WHITE" : "BLACK") << " POV]          │\n";
        std::cout << "└──────────────────────────────────────────────────────┘\n";

        board.unmakeMove(saved);
    }
} // EvaluationUtils
