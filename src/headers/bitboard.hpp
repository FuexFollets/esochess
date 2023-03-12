#ifndef ESOCHESS_BITBOARD_HPP
#define ESOCHESS_BITBOARD_HPP
#pragma once

#include <optional>
#include <array>
#include <cstdint>

namespace esochess::game {
    struct bitboard {
        private:

        enum Turn {White, Black};
        enum BitboardIndex {
            WhitePawn, WhiteKnight, WhiteBishop, WhiteRook, WhiteQueen, WhiteKing,
            BlackPawn, BlackKnight, BlackBishop, BlackRook, BlackQueen, BlackKing
        };

        std::array<std::uint64_t, 12> pieces_bit_board {};
        Turn turn {};

        struct {
            bool white_king {};
            bool white_queen {};
            bool black_king {};
            bool black_queen {};
        } can_castle {};

        std::optional<std::uint64_t> en_passant_square;

        std::size_t half_move_clock {};
        std::size_t full_move_clock {};

        public:
    };
}

#endif
