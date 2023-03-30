#ifndef ESOCHESS_BITBOARD_HPP
#define ESOCHESS_BITBOARD_HPP
#pragma once

namespace esochess {
    struct bitboard {
        enum Turn {White, Black, None};

        static constexpr const char* const starting_position_fen {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
    };
}
