# KNIGHTMARE

<img src="./logo.png">

A chess engine written in C++ just for fun and to learn. Move generation is complete and verified. Search and evaluation are next.

---

## How it works

### Board Representation

The board is represented using **bitboards** — one 64-bit integer per piece type per color. So `bitboards[WHITE][KNIGHT]` is a single `uint64_t` where each set bit represents a white knight on that square. This makes move generation extremely fast since most operations are just bitwise AND/OR/XOR.

Alongside the piece bitboards, three occupancy bitboards are maintained — one for white, one for black, and one for both combined. These are kept in sync on every `placePiece` and `removePiece` call.

A **mailbox** array runs alongside the bitboards — `mailBox[square]` gives the piece and color on that square in O(1). It's kept in sync with the bitboards on every `placePiece` and `removePiece` call and is used to avoid iterating all 12 bitboards when you just need to know what's on a square.

The board also tracks:
- Side to move
- Castle rights (stored as a 4-bit mask)
- En passant square (-1 if none)

### Attack Table Precomputation

Before any game logic runs, `PreMatchAttackComputation::init()` precomputes every possible attack from every square for every piece type. These are stored as static lookup tables.

For sliding pieces (rook, bishop, queen), attacks are split **by direction** — north, south, east, west, and the four diagonals. This is the key insight that makes `isSquareAttacked` efficient: instead of computing attacks on the fly, you cast a ray in each direction from the target square and check the nearest blocker.

**Important:** `init()` must be called before anything else in `main`. If you forget it, all attack tables are zero and no moves get generated — learned this the hard way.

### Move Generation

Move generation is split into two stages:

**Pseudo-legal moves** (`GeneratePseudoLegalMove`) — generates all moves a piece could make ignoring whether it leaves the king in check. Fast, no board state changes needed.

**Legal move filtering** (`LegalMoveFilter`) — for each pseudo-legal move, saves board state, makes the move, checks if the king is in check, then restores the board. Only moves that don't leave the king in check survive.

This save/restore pattern uses a `BoardState` struct that snapshots all bitboards, occupancies, mailbox, castle rights, en passant square, and side. Everything is `std::array` so restoring is just a direct assignment — no heap allocation.

Move lists use a fixed-size stack-allocated `MoveList` struct (218 entries — the proven maximum possible legal moves in any chess position) instead of `std::vector`. This eliminates heap allocation from the hot path entirely.

### isSquareAttacked

The core of legality checking. Given a square and a color, it asks: "is this square attacked by the enemy?"

The approach is **pretend to be the attacker**:
- Cast rook rays from the square → if you hit an enemy rook or queen, you're attacked
- Cast bishop rays from the square → if you hit an enemy bishop or queen, you're attacked
- Use knight attack table from the square → if an enemy knight sits there, you're attacked
- Use your own pawn attack mask from the square → if an enemy pawn sits there, you're attacked

Knight, pawn and king checks are a single bitwise AND against the relevant enemy piece bitboard — no loops at all. Ray checks for rooks and bishops still iterate directions but the inner traversal is replaced by a single AND against combined enemy pieces.

`isKingInCheck` is just a thin wrapper that finds the king's square and calls `isSquareAttacked`.

### Make / Unmake

`Board::makeMove` handles the mechanical side:
- Clear en passant square
- Handle captures (including en passant)
- Remove piece from source square, place on destination
- Update castle rights if king or rook moved
- Move the rook if castling
- Set en passant square if double pawn push

`Board::unmakeMove` restores the full `BoardState` snapshot. The reason these are separate functions rather than one combined function is search — minimax needs to make a move, recurse deeper, then unmake it. The make and unmake are separated by an entire recursive call stack.
`Board` does not know about attack tables. `MoveFunctions` does not mutate board state.

---

## Perft Results (Release build)

All standard perft positions pass. Times measured in Release mode (`-O3`). I also expect this to speed up as Magic BitBoards are not implemented yet.

| Position | Depth | Nodes | Time |
|---|---|---|---|
| Starting position | 6 | 119,060,324 | 15s |
| Kiwipete | 5 | 193,690,690 | 18s |
| Position 3 | 5 | 674,624 | 125ms |
| Position 4 | 4 | 422,333 | 57ms |
| Position 5 | 4 | 2,103,487 | 235ms |

Kiwipete is specifically designed to stress-test castling, en passant, and promotion edge cases. Passing it at depth 5 means 193 million positions all generated correctly.

A Stockfish comparison tool is included in `tests/` — it pipes commands to a local Stockfish installation and compares per-move node counts to isolate bugs to a specific position at depth 1.

---

## What's next

- [x] Move generation complete
- [x] All perft suites passing
- [x] Castle rights correctly updated on king/rook moves
- [x] Mailbox for O(1) piece lookup
- [x] Stack-allocated MoveList (no heap allocation in move gen)
- [ ] Minimax with alpha-beta pruning
- [ ] Simple evaluation (piece values)
- [ ] Make it play a legal game
- [ ] Move ordering
- [ ] Transposition table