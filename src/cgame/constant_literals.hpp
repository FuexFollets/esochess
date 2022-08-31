#pragma once

constexpr struct {
    int PAWN{0},
        KNIGHT{1},
        BISHOP{2},
        ROOK{3},
        QUEEN{4},
        KING{5};
} PTYPES;

constexpr struct {
    int WHITE{0},
        BLACK{1},
        BLANK{3};
} PCOLORS;

constexpr struct {
    int NORMAL{0},
        CASTLE{1},
        PROMOTION{2},
        EN_PASSANT{3};
} MTYPES;

constexpr struct {
    int KINGSIDE{0},
        QUEENSIDE{1};
} CTYPES;