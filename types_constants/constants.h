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

// squares that must be EMPTY for castling
constexpr U64 WHITE_KING_SIDE_CASTLE_EMPTY = (1ULL << 5) | (1ULL << 6);
constexpr U64 WHITE_QUEEN_SIDE_CASTLE_EMPTY = (1ULL << 1) | (1ULL << 2) | (1ULL << 3);
constexpr U64 BLACK_KING_SIDE_CASTLE_EMPTY = (1ULL << 61) | (1ULL << 62);
constexpr U64 BLACK_QUEEN_SIDE_CASTLE_EMPTY = (1ULL << 57) | (1ULL << 58) | (1ULL << 59);

// squares king passes through — must not be attacked (excludes b1/b8)
constexpr U64 WHITE_KING_SIDE_CASTLE_SAFE = (1ULL << 5) | (1ULL << 6);
constexpr U64 WHITE_QUEEN_SIDE_CASTLE_SAFE = (1ULL << 2) | (1ULL << 3);
constexpr U64 BLACK_KING_SIDE_CASTLE_SAFE = (1ULL << 61) | (1ULL << 62);
constexpr U64 BLACK_QUEEN_SIDE_CASTLE_SAFE = (1ULL << 58) | (1ULL << 59); // c8, d8 only — not b8

/**
 * If Using CLION
 * IGNORE THE X AND Y SHOWN IN THE CLION THAT IS MISLEADING
 */
inline std::vector<std::pair<int, int> > directions = {
    {1, 0}, // NORTH
    {-1, 0}, // SOUTH
    {0, 1}, // EAST
    {0, -1}, // WEST
    {1, 1}, // NORTH_EAST
    {1, -1}, // NORTH_WEST
    {-1, 1}, // SOUTH_EAST
    {-1, -1} // SOUTH_WEST
};

inline std::vector<std::pair<int, int> > knight_directions = {
    {2, -1}, {2, 1}, {-2, 1}, {-2, -1},
    {1, -2}, {-1, -2}, {1, 2}, {-1, 2}
};

inline std::map<char, ColorPiece> fenEnumMapping = {
    {'P', {WHITE, PAWN}}, {'N', {WHITE, KNIGHT}}, {'B', {WHITE, BISHOP}},
    {'R', {WHITE, ROOK}}, {'Q', {WHITE, QUEEN}}, {'K', {WHITE, KING}},
    {'p', {BLACK, PAWN}}, {'n', {BLACK, KNIGHT}}, {'b', {BLACK, BISHOP}},
    {'r', {BLACK, ROOK}}, {'q', {BLACK, QUEEN}}, {'k', {BLACK, KING}}
};

inline uint16_t pawnMasks[2] = {0b1111'1111'0000'0000, 0b0000'0000'1111'1111};

struct PerftResult {
    int depth;
    long long expected;
};

inline const std::vector<PerftResult> STARTING_POSITION_PERFT = {
    {1, 20},
    {2, 400},
    {3, 8'902},
    {4, 197'281},
    {5, 4'865'609},
    {6, 119'060'324}
};

// inline const std::vector<PerftResult> KIWIPETE_PERFT = {
//     {1, 48},
//     {2, 2'039},
//     {3, 97'862},
//     {4, 4'085'603},
//     {5, 193'690'690},
//     {6, 8'031'630'681}
// };
inline const std::vector<PerftResult> KIWIPETE_PERFT = {
    {1, 48},
    {2, 2'039},
    {3, 97'862},
    {4, 4'085'603},
    {5, 193'690'690},
};
inline const std::vector<PerftResult> POSITION3_PERFT = {
    {1, 14},
    {2, 191},
    {3, 2'812},
    {4, 43'238},
    {5, 674'624}
};
inline const std::vector<PerftResult> POSITION4_PERFT = {
    {1, 6},
    {2, 264},
    {3, 9'467},
    {4, 422'333}
};
inline const std::vector<PerftResult> POSITION5_PERFT = {
    {1, 44},
    {2, 1486},
    {3, 62'379},
    {4, 2'103'487}
};
inline const std::array<std::string, 6> moveTypeString = {
    "SIMPLE", "PROMOTION", "CASTLE_KING_SIDE",
    "CASTLE_QUEEN_SIDE", "EN_PASSANT", "DOUBLE_PAWN_MOVE"
};

inline std::string NORMAL_START_POSITION_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
inline std::string KIWIPETE_PERFT_START_FEN = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
inline std::string POSITION3_FEN = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -";
inline std::string POSITION4_FEN = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq -";
inline std::string POSITION5_FEN = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ -";


#endif //CONSTANTS_H
