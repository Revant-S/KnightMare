//
// Created by revant-sinha on 3/8/26.
//

#ifndef GENERATEPSEUDOLEGALMOVE_H
#define GENERATEPSEUDOLEGALMOVE_H
#include "../../types_constants/types.h"
#include "../../board/Board.h"

namespace GenerateLegalMove {
    std::vector<Move> getKnightPseudoLegalMoves(Board &board);

    std::vector<Move> getRookPseudoLegalMoves(Board &board);

    std::vector<Move> getBishopPseudoLegalMoves(Board &board);

    std::vector<Move> getQueenPseudoLegalMoves(Board &board);

    std::vector<Move> getKingPseudoLegalMoves(Board &board);

    std::vector<Move> getPawnPseudoLegalMoves(const Board &board);
} // GenerateLegalMove

#endif //GENERATEPSEUDOLEGALMOVE_H
