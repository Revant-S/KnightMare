//
// Created by revant-sinha on 3/8/26.
//

#ifndef GENERATEPSEUDOLEGALMOVE_H
#define GENERATEPSEUDOLEGALMOVE_H
#include "../../types_constants/types.h"
#include "../../board/Board.h"

namespace GeneratePseudoLegalMove {
    void getKnightPseudoLegalMoves(Board &board, MoveList &moves);

    void getRookPseudoLegalMoves(Board &board, MoveList &moves);

    void getBishopPseudoLegalMoves(Board &board, MoveList &moves);

    void getQueenPseudoLegalMoves(Board &board, MoveList &moves);

    void getKingPseudoLegalMoves(Board &board, MoveList &moves);

    void getPawnPseudoLegalMoves(Board &board, MoveList &moves);
} // GenerateLegalMove

#endif //GENERATEPSEUDOLEGALMOVE_H
