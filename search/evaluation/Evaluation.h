//
// Created by revant-sinha on 3/29/26.
//

#ifndef EVALUTATION_H
#define EVALUTATION_H
#include "../../board/Board.h"

namespace Evaluation {
    struct TaperedScore {
        int midGame = 0;
        int endGame = 0;

        TaperedScore &operator+=(const TaperedScore &other) {
            midGame += other.midGame;
            endGame += other.endGame;
            return *this;
        }

        TaperedScore operator-(const TaperedScore &other) const {
            return {midGame - other.midGame, endGame - other.endGame};
        }
    };

    int evaluate(Board &board);

    int gamePhase(const Board &board);

    bool isEndGameReached(const Board &board);

    bool isInsufficientMaterial(const Board &board);

    int blendByPhase(const TaperedScore &score, int phase);

    TaperedScore materialScore(const Board &board, Color side);

    TaperedScore pieceSquareScore(const Board &board, Color side);

    TaperedScore pawnStructureScore(const Board &board, Color side);

    TaperedScore pieceActivityScore(const Board &board, Color side);

    TaperedScore kingSafetyScore(const Board &board, Color side);

    int mopUpScore(const Board &board);
} // Evaluation

#endif //EVALUTATION_H
