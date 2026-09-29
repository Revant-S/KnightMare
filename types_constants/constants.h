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

// Material Weights for evaluation in the order of enum
// The reasoning:

// Pawn   = 100   baseline unit, everything relative to this
// Knight = 320   slightly less than bishop in open positions
// Bishop = 330   slightly better than knight, bishop pair is strong
// Rook   = 500   worth roughly 5 pawns, a rook and pawn beats two minor pieces
// Queen  = 900   roughly rook + bishop
// King   = 20000  never actually captured, just needs to be so large
//                  that no material gain ever justifies losing it

inline std::array<int, 6> materialWeight = {
    100,
    320,
    330,
    500,
    900,
    20000
};

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
inline const std::string POSITION5_FEN = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ -";
inline std::array<std::array<int, 64>, 7> pstTable =
{
    {

        // PAWN
        {
            {
                0, 0, 0, 0, 0, 0, 0, 0,
                50, 50, 50, 50, 50, 50, 50, 50,
                10, 10, 20, 30, 30, 20, 10, 10,
                5, 5, 10, 25, 25, 10, 5, 5,
                0, 0, 0, 20, 20, 0, 0, 0,
                5, -5, -10, 0, 0, -10, -5, 5,
                5, 10, 10, -20, -20, 10, 10, 5,
                0, 0, 0, 0, 0, 0, 0, 0
            }
        },

        // KNIGHT
        {
            {
                -50, -40, -30, -30, -30, -30, -40, -50,
                -40, -20, 0, 0, 0, 0, -20, -40,
                -30, 0, 10, 15, 15, 10, 0, -30,
                -30, 5, 15, 20, 20, 15, 5, -30,
                -30, 0, 15, 20, 20, 15, 0, -30,
                -30, 5, 10, 15, 15, 10, 5, -30,
                -40, -20, 0, 5, 5, 0, -20, -40,
                -50, -40, -30, -30, -30, -30, -40, -50
            }
        },

        // BISHOP
        {
            {
                -20, -10, -10, -10, -10, -10, -10, -20,
                -10, 5, 0, 0, 0, 0, 5, -10,
                -10, 10, 10, 10, 10, 10, 10, -10,
                -10, 0, 10, 10, 10, 10, 0, -10,
                -10, 5, 5, 10, 10, 5, 5, -10,
                -10, 0, 5, 10, 10, 5, 0, -10,
                -10, 0, 0, 0, 0, 0, 0, -10,
                -20, -10, -10, -10, -10, -10, -10, -20
            }
        },

        // ROOK
        {
            {
                0, 0, 5, 10, 10, 5, 0, 0,
                -5, 0, 0, 0, 0, 0, 0, -5,
                -5, 0, 0, 0, 0, 0, 0, -5,
                -5, 0, 0, 0, 0, 0, 0, -5,
                -5, 0, 0, 0, 0, 0, 0, -5,
                -5, 0, 0, 0, 0, 0, 0, -5,
                5, 10, 10, 10, 10, 10, 10, 5,
                0, 0, 5, 10, 10, 5, 0, 0
            }
        },

        // QUEEN
        {
            {
                -20, -10, -10, -5, -5, -10, -10, -20,
                -10, 0, 5, 0, 0, 0, 0, -10,
                -10, 5, 5, 5, 5, 5, 0, -10,
                -5, 0, 5, 5, 5, 5, 0, -5,
                0, 0, 5, 5, 5, 5, 0, -5,
                -10, 0, 5, 5, 5, 5, 0, -10,
                -10, 0, 0, 0, 0, 0, 0, -10,
                -20, -10, -10, -5, -5, -10, -10, -20
            }
        },

        // KING MIDGAME
        {
            {
                -30, -40, -40, -50, -50, -40, -40, -30,
                -30, -40, -40, -50, -50, -40, -40, -30,
                -30, -40, -40, -50, -50, -40, -40, -30,
                -30, -40, -40, -50, -50, -40, -40, -30,
                -20, -30, -30, -40, -40, -30, -30, -20,
                -10, -20, -20, -20, -20, -20, -20, -10,
                20, 20, 0, 0, 0, 0, 20, 20,
                20, 30, 10, 0, 0, 10, 30, 20
            }
        },

        // KING ENDGAME
        {
            {
                -50, -40, -30, -20, -20, -30, -40, -50,
                -30, -20, -10, 0, 0, -10, -20, -30,
                -30, -10, 20, 30, 30, 20, -10, -30,
                -30, -10, 30, 40, 40, 30, -10, -30,
                -30, -10, 30, 40, 40, 30, -10, -30,
                -30, -10, 20, 30, 30, 20, -10, -30,
                -30, -30, 0, 0, 0, 0, -30, -30,
                -50, -30, -30, -30, -30, -30, -30, -50
            }
        }

    }
};
inline constexpr std::array<int, 6> endGameMaterialWeight = {
    120, 290, 310, 540, 960, 20000
};

// Pieces laid out from white's view, rank 8 first, same as pstTable
inline constexpr std::array<int, 64> pawnEndGameTable = {
    0, 0, 0, 0, 0, 0, 0, 0,
    80, 80, 80, 80, 80, 80, 80, 80,
    50, 50, 50, 50, 50, 50, 50, 50,
    30, 30, 30, 30, 30, 30, 30, 30,
    15, 15, 15, 15, 15, 15, 15, 15,
    5, 5, 5, 5, 5, 5, 5, 5,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

inline constexpr std::array<int, 8> passedPawnMidGameBonus = {0, 5, 10, 15, 25, 40, 60, 0};
inline constexpr std::array<int, 8> passedPawnEndGameBonus = {0, 10, 15, 25, 45, 75, 120, 0};

inline constexpr std::array<int, 6> gamePhaseWeight = {0, 1, 1, 2, 4, 0};
inline constexpr int TOTAL_GAME_PHASE = 24;

inline constexpr U64 FILE_A_MASK = 0x0101010101010101ULL;

inline constexpr int MAX_SEARCH_DEPTH = 64;

inline constexpr std::array<U64, 64> ROOK_MAGIC_NUMBERS = {
    0x008002E8C0001080ULL, 0xD040004010002000ULL, 0x4180200080100088ULL, 0x01800D2800811000ULL,
    0x2200201008040200ULL, 0x0200084104500200ULL, 0x0A00480689220004ULL, 0x0300008421430012ULL,
    0x0000800080204010ULL, 0x0140802000804004ULL, 0x000C801008200080ULL, 0x0000801000080080ULL,
    0x0100800400080080ULL, 0x0012000805020010ULL, 0x0101000200010004ULL, 0x0598800050800100ULL,
    0x0021A1800A844008ULL, 0x0002424000201004ULL, 0x0000848020001000ULL, 0x100242000A002010ULL,
    0x2146020008112004ULL, 0x000A008080020400ULL, 0x2002040042480150ULL, 0x8445060004004481ULL,
    0x0380005040002000ULL, 0x00502000C0100040ULL, 0x1060100080200080ULL, 0x0040401200220008ULL,
    0x1004041100080101ULL, 0x00A2000200041008ULL, 0x0080100400010208ULL, 0x0000008200040061ULL,
    0x4080002000400044ULL, 0x00900020044003C0ULL, 0x0010002000801080ULL, 0x0002002212000A40ULL,
    0x0000080080800401ULL, 0x4018800400800200ULL, 0x1001020804000110ULL, 0xE100004102000084ULL,
    0x04007A8040008000ULL, 0x8020200050004000ULL, 0x003004080020A001ULL, 0x0080210090050008ULL,
    0x4048008004008008ULL, 0x0012001008020004ULL, 0x0500102221040008ULL, 0x0060108064020001ULL,
    0x4880004000200040ULL, 0x02C1201008400540ULL, 0xC800200084900680ULL, 0x0011831000080480ULL,
    0x1410040080280280ULL, 0x0122504060040801ULL, 0x000C223148300400ULL, 0x00050000A0420100ULL,
    0xA000402100800011ULL, 0x0000201089004001ULL, 0x4820002010400901ULL, 0x0810001021008815ULL,
    0x80010004103A4801ULL, 0x026200181011A402ULL, 0x3020010210080084ULL, 0x2100040110218642ULL,
};

inline constexpr std::array<U64, 64> BISHOP_MAGIC_NUMBERS = {
    0x0040011404048028ULL, 0x40A80810C4820000ULL, 0x02881805608A0000ULL, 0x0184041880000220ULL,
    0x0501104000400031ULL, 0xC001040241400180ULL, 0x0204012C82203400ULL, 0x20450C0104010403ULL,
    0x0104400428108100ULL, 0x0008200A248B0504ULL, 0x0001042408820000ULL, 0x40000C3404801220ULL,
    0x1508240521004002ULL, 0x0851010420040008ULL, 0x8004010410440405ULL, 0x0114042C01045008ULL,
    0x0020001044811800ULL, 0x00020004100C5101ULL, 0x04100008044440C8ULL, 0x8030220802404018ULL,
    0x0042000C00940882ULL, 0x0081002A10020100ULL, 0x0924231044024800ULL, 0x0101100080880120ULL,
    0x8820098010020880ULL, 0x8210020804440400ULL, 0x0000240008104400ULL, 0x0404080101010500ULL,
    0x0001010004104010ULL, 0x2084298404100400ULL, 0x0004011004008201ULL, 0x0000820089005202ULL,
    0x0004120A80409040ULL, 0x0022010480A03800ULL, 0x1001080100181040ULL, 0x4000040400080120ULL,
    0x0142040808040020ULL, 0x0001300100208040ULL, 0x80C1110110420800ULL, 0x0018020020008089ULL,
    0x0401082005011200ULL, 0x0040420804426008ULL, 0x1401010802028104ULL, 0x9150404010400200ULL,
    0x8001401C08200100ULL, 0x8410901000C02288ULL, 0x8230C14904001100ULL, 0x1010010041090090ULL,
    0x01011410040500D0ULL, 0x4442212808048100ULL, 0x0108004608041000ULL, 0x4000000042020005ULL,
    0x0000005012088040ULL, 0x0000A1A81000C008ULL, 0x00061004110408B1ULL, 0x0004080820408010ULL,
    0x0400C20841201080ULL, 0x0008810412210403ULL, 0x00000002004A0880ULL, 0x1001020100208808ULL,
    0x4008141228103400ULL, 0x0900804050120093ULL, 0x480008901000A100ULL, 0x0020080080808201ULL,
};

#endif //CONSTANTS_H
