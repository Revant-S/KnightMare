//
// Created by revant-sinha on 3/6/26.
//

#include "Board.h"
#include "../types_constants/types.h"
#include <bitset>
#include <random>
#include <cstdint>

#include "../utils/utils.h"

Board::Board(const std::string &fenString) {
    int rank = 7, file = 0;
    int sideIndex = 0;
    mailBox.fill({BOTH, PAWN});
    for (const unsigned char piece: fenString) {
        sideIndex++;
        if (piece == ' ')break;
        if (piece == '/') {
            rank -= 1;
            file = 0;
        } else if (std::isdigit(piece)) {
            const int skips = piece - '0';
            file += skips;
        } else {
            int boardIndex = rank * 8 + file;
            if (boardIndex >= 0 && boardIndex < 64) {
                const int color = fenEnumMapping[piece].color;
                const int boardPiece = fenEnumMapping[piece].piece;
                bitboards[color][boardPiece] |= (static_cast<U64>(1) << boardIndex);
                occupancies[color] |= (static_cast<U64>(1) << boardIndex);
                occupancies[BOTH] |= (static_cast<U64>(1) << boardIndex);
                mailBox[boardIndex] = {static_cast<Color>(color), static_cast<Piece>(boardPiece)};
                file++;
            } else {
                break;
            }
        }
    }
    if (fenString[sideIndex] == 'w') {
        side = WHITE;
    } else {
        side = BLACK;
    }
    int castleIndex = sideIndex + 2;

    castleRights = 0b0000;
    while (castleIndex < fenString.length() && fenString[castleIndex] != ' ') {
        if (const char symbol = fenString[castleIndex]; symbol == '-') {
            break;
        } else if (symbol == 'K') {
            castleRights |= WHITE_KING_SIDE_CASTLE_MASK;
        } else if (symbol == 'Q') {
            castleRights |= WHITE_QUEEN_SIDE_CASTLE_MASK;
        } else if (symbol == 'k') {
            castleRights |= BLACK_KING_SIDE_CASTLE_MASK;
        } else if (symbol == 'q') {
            castleRights |= BLACK_QUEEN_SIDE_CASTLE_MASK;
        }
        castleIndex++;
    }
    int enpassantFenIndex = castleIndex;
    while (enpassantFenIndex < fenString.length() && fenString[enpassantFenIndex] == ' ')
        enpassantFenIndex++;

    if (fenString[enpassantFenIndex] != '-') {
        int squareFile = fenString[enpassantFenIndex] - 'a';
        enpassantFenIndex++;
        int squareRank = fenString[enpassantFenIndex] - '1';
        enPassantSquare = squareRank * BOARD_WIDTH + squareFile;
    }
}

void Board::toggle_side() {
    side == WHITE ? side = BLACK : side = WHITE;
}

void Board::print_board() const {
    std::cout << "\n";

    for (int rank = 7; rank >= 0; rank--) {
        std::cout << rank + 1 << "  ";
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            char pieceChar = '.';
            U64 squareMask = 1ULL << square;
            if (bitboards[WHITE][PAWN] & squareMask) pieceChar = 'P';
            else if (bitboards[WHITE][KNIGHT] & squareMask) pieceChar = 'N';
            else if (bitboards[WHITE][BISHOP] & squareMask) pieceChar = 'B';
            else if (bitboards[WHITE][ROOK] & squareMask) pieceChar = 'R';
            else if (bitboards[WHITE][QUEEN] & squareMask) pieceChar = 'Q';
            else if (bitboards[WHITE][KING] & squareMask) pieceChar = 'K';

            else if (bitboards[BLACK][PAWN] & squareMask) pieceChar = 'p';
            else if (bitboards[BLACK][KNIGHT] & squareMask) pieceChar = 'n';
            else if (bitboards[BLACK][BISHOP] & squareMask) pieceChar = 'b';
            else if (bitboards[BLACK][ROOK] & squareMask) pieceChar = 'r';
            else if (bitboards[BLACK][QUEEN] & squareMask) pieceChar = 'q';
            else if (bitboards[BLACK][KING] & squareMask) pieceChar = 'k';

            std::cout << pieceChar << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n   a b c d e f g h\n\n";
    std::cout << "Side to move: " << (side == WHITE ? "White" : "Black") << "\n";
}

U64 Board::getPieceBitBoard(const Piece piece, const Color color) const {
    if (piece > KING || color > BLACK) {
        return static_cast<U64>(0);
    }
    return bitboards[color][piece];
}

U64 Board::getOccupancies(const Color color) const {
    return occupancies[color];
}

void Board::getAndPrintPawnMovePermissions() {
    std::cout << "permissions mask\n";
    std::cout << std::bitset<16>(doublePawnMoveRight) << "\n";;

    std::cout << "Black Pawn Double Move Permissions" << "\n";
    for (int i = 0; i < boardWidth; i++) {
        if ((doublePawnMoveRight & (static_cast<int16_t>(1) << i))) {
            std::cout << "p" << " ";
        } else {
            std::cout << "." << " ";
        }
    }
    std::cout << "\n White Pawn Double Move Permissions" << "\n";
    for (int i = boardWidth; i < 16; i++) {
        if ((doublePawnMoveRight & (static_cast<int16_t>(1) << i))) {
            std::cout << "P" << " ";
        } else {
            std::cout << "." << " ";
        }
    }
}

U64 Board::getDoubleMovePawnPermissions(Color color) const {
    return pawnMasks[color] & doublePawnMoveRight;
}

inline void Board::setEnPassantSquare(const int squareIndex) {
    if (squareIndex > 63 || squareIndex < 0) {
        return;
    };
    enPassantSquare = squareIndex;
}

inline void Board::clearEnPassantSquare() {
    enPassantSquare = -1;
}

int Board::getEnpassantSquare() const {
    return enPassantSquare;
}

void Board::setPieceBitBoard(Color color, Piece piece, U64 newBitBoard) {
    bitboards[color][piece] = newBitBoard;
}

void Board::setOccupanciesBitBoard(Color color, U64 newOccupancy) {
    occupancies[color] = newOccupancy;
}

void Board::removePiece(const int square, Piece piece, Color color) {
    const U64 pieceRemovalBitMask = static_cast<U64>(1) << square;
    bitboards[color][piece] ^= pieceRemovalBitMask;
    occupancies[color] ^= pieceRemovalBitMask;
    occupancies[BOTH] ^= pieceRemovalBitMask;
    mailBox[square] = {
        BOTH,
        PAWN
    };
}

void Board::placePiece(const int square, Piece piece, Color color) {
    const U64 placeMoveBitMask = static_cast<U64>(1) << square;
    bitboards[color][piece] |= placeMoveBitMask;
    occupancies[color] |= placeMoveBitMask;
    occupancies[BOTH] |= placeMoveBitMask;
    mailBox[square] = {
        color,
        piece
    };
}

ColorPiece Board::getPieceOnTheIndex(const int index) const {
    return mailBox[index];
}

void Board::generateKeysForHashing() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> distrib(0, UINT64_MAX);
    for (int piece = PAWN; piece <= KING; piece++) {
        for (int index = 0; index < 64; index++) {
            for (int side = WHITE; side <= BLACK; side++) {
                pieceKey[piece][index][side] = distrib(gen);
            }
        }
    }
}


U64 Board::generatePositionHash() {
    U64 allOccupancies = occupancies[BOTH];
    const int firstPosition = Utils::getLSB(allOccupancies);
    auto [color, piece] = mailBox[firstPosition];
    U64 hashKey = getPieceKey(piece, color, firstPosition);
    Utils::popLSB(allOccupancies);
    while (allOccupancies) {
        int position = Utils::getLSB(allOccupancies);
        Utils::popLSB(allOccupancies);
        hashKey ^= getPieceKey(mailBox[position].piece, mailBox[position].color, position);
    }
    return hashKey;
}

U64 Board::getPieceKey(Piece piece, Color color, int position) {
    return pieceKey[piece][position][color];
}

void Board::handleCaptureForMove(Move &move) {
    const Color enemy = (side == WHITE) ? BLACK : WHITE;
    const U64 enemyOccupancy = occupancies[enemy];
    const int destinationSquare = move.to;
    auto [enemyColor, enemyPiece] = getPieceOnTheIndex(destinationSquare);
    const U64 destinationBitMask = static_cast<U64>(1) << destinationSquare;
    const MoveType moveType = move.moveType;
    if (moveType == SIMPLE || moveType == PROMOTION) {
        if (destinationBitMask & enemyOccupancy) {
            removePiece(move.to, enemyPiece, enemyColor);
            if (enemyPiece == ROOK) {
                if (enemyColor == WHITE && destinationSquare == WHITE_ROOK_KING_SIDE_INDEX) {
                    clearCastleRight(WHITE, KING);
                } else if (enemyColor == WHITE && destinationSquare == WHITE_ROOK_QUEEN_SIDE_INDEX) {
                    clearCastleRight(WHITE, QUEEN);
                } else if (enemyColor == BLACK && destinationSquare == BLACK_ROOK_KING_SIDE_INDEX) {
                    clearCastleRight(BLACK, KING);
                } else if (enemyColor == BLACK && destinationSquare == BLACK_ROOK_QUEEN_SIDE_INDEX) {
                    clearCastleRight(BLACK, QUEEN);
                }
            }
        }
        return;
    }

    if (moveType == EN_PASSANT) {
        int removeSquareIndex = enemy == WHITE ? enPassantSquare + 8 : enPassantSquare - 8;
        removePiece(removeSquareIndex, PAWN, enemy);
    }
}

BoardState Board::saveState() {
    return {
        bitboards,
        occupancies,
        mailBox,
        enPassantSquare,
        castleRights,
        side
    };
}

void Board::unmakeMove(const BoardState savedState) {
    bitboards = savedState.bitboards;
    occupancies = savedState.occupancies;
    side = savedState.side;
    enPassantSquare = savedState.enPassantSquare;
    castleRights = savedState.castleRights;
    mailBox = savedState.mailBox;
}

void Board::clearALlCastleRightsOf(Color color) {
    if (color == WHITE) {
        castleRights &= ~0b0011;
    } else if (color == BLACK) {
        castleRights &= ~0b1100;
    }
}

void Board::clearCastleRight(Color color, Piece piece) {
    assert(piece == KING || piece == QUEEN);
    if (color == WHITE) {
        if (piece == KING) {
            castleRights &= ~WHITE_KING_SIDE_CASTLE_MASK;
        } else {
            castleRights &= ~WHITE_QUEEN_SIDE_CASTLE_MASK;
        }
    } else {
        if (piece == KING) {
            castleRights &= ~BLACK_KING_SIDE_CASTLE_MASK;
        } else {
            castleRights &= ~BLACK_QUEEN_SIDE_CASTLE_MASK;
        }
    }
}

void Board::makeMove(Move &move) {
    Color color = move.colorOfPieceToMove;
    handleCaptureForMove(move);
    removePiece(move.from, move.piece, color);
    MoveType moveType = move.moveType;
    if (moveType != PROMOTION) {
        placePiece(move.to, move.piece, color);
    } else {
        placePiece(move.to, move.promoteTo, color);
    }
    clearEnPassantSquare();
    Piece piece = move.piece;
    Color moveColor = move.colorOfPieceToMove;
    if (move.piece == KING) {
        clearALlCastleRightsOf(color);
    }
    if (moveType == DOUBLE_PAWN_MOVE) {
        setEnPassantSquare(color == WHITE ? move.to - BOARD_WIDTH : move.to + BOARD_WIDTH);
        return;
    }
    // verify the logic
    if (piece == ROOK) {
        if (moveColor == WHITE) {
            if (move.from == WHITE_QUEEN_SIDE_ROOK_FROM) {
                clearCastleRight(WHITE, QUEEN);
            } else if (move.from == WHITE_KING_SIDE_ROOK_FROM) {
                clearCastleRight(WHITE, KING);
            }
        } else {
            if (move.from == BLACK_QUEEN_SIDE_ROOK_FROM) {
                clearCastleRight(BLACK, QUEEN);
            } else if (move.from == BLACK_KING_SIDE_ROOK_FROM) {
                clearCastleRight(BLACK, KING);
            }
        }
    }

    if (moveType == CASTLE_KING_SIDE || moveType == CASTLE_QUEEN_SIDE) {
        int rookFrom, rookTo;
        if (moveType == CASTLE_KING_SIDE) {
            rookFrom = (color == WHITE) ? WHITE_KING_SIDE_ROOK_FROM : BLACK_KING_SIDE_ROOK_FROM;
            rookTo = (color == WHITE) ? WHITE_KING_SIDE_ROOK_TO : BLACK_KING_SIDE_ROOK_TO;
        } else {
            rookFrom = (color == WHITE) ? WHITE_QUEEN_SIDE_ROOK_FROM : BLACK_QUEEN_SIDE_ROOK_FROM;
            rookTo = (color == WHITE) ? WHITE_QUEEN_SIDE_ROOK_TO : BLACK_QUEEN_SIDE_ROOK_TO;
        }
        removePiece(rookFrom, ROOK, color);
        placePiece(rookTo, ROOK, color);
    }
}

std::string Board::toFEN() const {
    std::string fen;

    // 1. Piece placement
    for (int rank = 7; rank >= 0; rank--) {
        int emptyCount = 0;

        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            auto [color, piece] = mailBox[square];

            if (color == BOTH) {
                emptyCount++;
            } else {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }

                char pieceChar = ' ';
                switch (piece) {
                    case PAWN: pieceChar = 'p';
                        break;
                    case KNIGHT: pieceChar = 'n';
                        break;
                    case BISHOP: pieceChar = 'b';
                        break;
                    case ROOK: pieceChar = 'r';
                        break;
                    case QUEEN: pieceChar = 'q';
                        break;
                    case KING: pieceChar = 'k';
                        break;
                }

                if (color == WHITE) {
                    pieceChar = std::toupper(pieceChar);
                }

                fen += pieceChar;
            }
        }

        if (emptyCount > 0) {
            fen += std::to_string(emptyCount);
        }

        if (rank > 0) {
            fen += '/';
        }
    }

    // 2. Side to move
    fen += ' ';
    fen += (side == WHITE ? 'w' : 'b');

    // 3. Castling rights
    fen += ' ';
    std::string castling = "";

    if (castleRights & WHITE_KING_SIDE_CASTLE_MASK) castling += 'K';
    if (castleRights & WHITE_QUEEN_SIDE_CASTLE_MASK) castling += 'Q';
    if (castleRights & BLACK_KING_SIDE_CASTLE_MASK) castling += 'k';
    if (castleRights & BLACK_QUEEN_SIDE_CASTLE_MASK) castling += 'q';

    fen += (castling.empty() ? "-" : castling);

    // 4. En passant
    fen += ' ';
    if (enPassantSquare == -1) {
        fen += '-';
    } else {
        int file = enPassantSquare % 8;
        int rank = enPassantSquare / 8;

        fen += static_cast<char>('a' + file);
        fen += static_cast<char>('1' + rank);
    }

    // 5. Halfmove clock (not tracked yet)
    fen += " 0";

    // 6. Fullmove number (not tracked yet)
    fen += " 1";

    return fen;
}

int Board::getCastleRights(Color color) const {
    if (color != WHITE && color != BLACK) return -1;
    if (color == WHITE) {
        return castleRights & (WHITE_KING_SIDE_CASTLE_MASK | WHITE_QUEEN_SIDE_CASTLE_MASK);
    }

    return castleRights & (BLACK_KING_SIDE_CASTLE_MASK | BLACK_QUEEN_SIDE_CASTLE_MASK);
}

Color Board::getSide() const {
    return side;
}
