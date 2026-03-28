//
// Created by revant-sinha on 3/8/26.
//

#ifndef GENERATEPSEUDOLEGALMOVE_H
#define GENERATEPSEUDOLEGALMOVE_H
#include "../../types_constants/types.h"
#include "../../board/Board.h"

namespace GeneratePseudoLegalMove {
    MoveList getKnightPseudoLegalMoves(Board &board);

    MoveList getRookPseudoLegalMoves(Board &board);

    MoveList getBishopPseudoLegalMoves(Board &board);

    MoveList getQueenPseudoLegalMoves(Board &board);

    MoveList getKingPseudoLegalMoves(Board &board);

    MoveList getPawnPseudoLegalMoves(Board &board);
} // GenerateLegalMove

#endif //GENERATEPSEUDOLEGALMOVE_H
