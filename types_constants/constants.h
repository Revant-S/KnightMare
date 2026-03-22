//
// Created by revant-sinha on 3/6/26.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "types.h"

constexpr int BOARD_WIDTH = 8;
constexpr int BOARD_HEIGHT = 8;
constexpr int BLACK_BACK_RANK_THRESHOLD_INDEX = 56;
constexpr int WHITE_BACK_RANK_THRESHOLD_INDEX = 7;
constexpr int FINAL_SQUARE_INDEX = 63;
constexpr int FIRST_SQUARE_INDEX = 0;
constexpr int WHITE_KING_SIDE_CASTLE_MASK = 0b1;
constexpr int WHITE_QUEEN_SIDE_CASTLE_MASK = 0b10;
constexpr int BLACK_KING_SIDE_CASTLE_MASK = 0b100;
constexpr int BLACK_QUEEN_SIDE_CASTLE_MASK = 0b1000;
constexpr int WHITE_KING_SQUARE = 4;
constexpr int BLACK_KING_SQUARE = 60;
constexpr int BLACK_ROOK_QUEEN_SIDE_INDEX = 56;
constexpr int BLACK_ROOK_KING_SIDE_INDEX = 63;
constexpr int WHITE_ROOK_QUEEN_SIDE_INDEX = 0;
constexpr int WHITE_ROOK_KING_SIDE_INDEX = 7;
constexpr int BLACK_KING_QUEEN_SIDE_CASTLE_DESTINATION = 58;
constexpr int BLACK_KING_KING_SIDE_CASTLE_DESTINATION = 62;
constexpr int WHITE_KING_QUEEN_SIDE_CASTLE_DESTINATION = 2;
constexpr int WHITE_KING_KING_SIDE_CASTLE_DESTINATION = 6;
// castling rook squares
constexpr int WHITE_KING_SIDE_ROOK_FROM = 7;
constexpr int WHITE_KING_SIDE_ROOK_TO = 5;
constexpr int WHITE_QUEEN_SIDE_ROOK_FROM = 0;
constexpr int WHITE_QUEEN_SIDE_ROOK_TO = 3;
constexpr int BLACK_KING_SIDE_ROOK_FROM = 63;
constexpr int BLACK_KING_SIDE_ROOK_TO = 61;
constexpr int BLACK_QUEEN_SIDE_ROOK_FROM = 56;
constexpr int BLACK_QUEEN_SIDE_ROOK_TO = 59;
constexpr U64 WHITE_KING_SIDE_CASTLE_EMPTY = (1ULL << 5) | (1ULL << 6);
constexpr U64 WHITE_QUEEN_SIDE_CASTLE_EMPTY = (1ULL << 1) | (1ULL << 2) | (1ULL << 3);
constexpr U64 BLACK_KING_SIDE_CASTLE_EMPTY = (1ULL << 61) | (1ULL << 62);
constexpr U64 BLACK_QUEEN_SIDE_CASTLE_EMPTY = (1ULL << 57) | (1ULL << 58) | (1ULL << 59);

/**
 * If Using CLION
 * IGNORE THE X AND Y SHOWN IN THE CLION THAT IS MISLEADING
 */
inline std::vector<std::pair<int, int> > directions = {
    // Rooks
    {1, 0}, // NORTH (Rank +1, File 0)
    {-1, 0}, // SOUTH (Rank -1, File 0)
    {0, 1}, // EAST  (Rank 0, File +1)
    {0, -1}, // WEST  (Rank 0, File -1)
    // Bishops
    {1, 1}, // NORTH_EAST (Rank +1, File +1)
    {1, -1}, // NORTH_WEST (Rank +1, File -1)
    {-1, 1}, // SOUTH_EAST (Rank -1, File +1)
    {-1, -1} // SOUTH_WEST (Rank -1, File -1)
};

inline std::vector<std::pair<int, int> > knight_directions = {
    {2, -1}, {2, 1}, {-2, 1}, {-2, -1},
    {1, -2}, {-1, -2}, {1, 2}, {-1, 2}
};


inline std::map<char, ColorPiece> fenEnumMapping = {
    // White Pieces (Uppercase)
    {'P', {WHITE, PAWN}},
    {'N', {WHITE, KNIGHT}},
    {'B', {WHITE, BISHOP}},
    {'R', {WHITE, ROOK}},
    {'Q', {WHITE, QUEEN}},
    {'K', {WHITE, KING}},

    // Black Pieces (Lowercase)
    {'p', {BLACK, PAWN}},
    {'n', {BLACK, KNIGHT}},
    {'b', {BLACK, BISHOP}},
    {'r', {BLACK, ROOK}},
    {'q', {BLACK, QUEEN}},
    {'k', {BLACK, KING}}
};

inline uint16_t pawnMasks[2] = {0b1111'1111'0000'0000, 0b0000'0000'1111'1111};


struct PerftResult {
    int depth;
    long long expected;
};

inline const std::vector<PerftResult> STARTING_POSITION_PERFT = {
    {1, 20},
    {2, 400},
    {3, 8902},
    {4, 197281},
    {5, 4865609},
    {6, 119060324}
};

inline const std::vector<PerftResult> KIWIPETE_PERFT = {
    {1, 48},
    {2, 2039},
    {3, 97862},
    {4, 4085603},
    {5, 193690690}
};

inline std::string NORMAL_START_POSITION_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
inline std::string KIWIPETE_PERFT_START_FEN = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";


#endif //CONSTANTS_H
