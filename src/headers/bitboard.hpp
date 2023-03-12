#ifndef ESOCHESS_BITBOARD_HPP
#define ESOCHESS_BITBOARD_HPP
#pragma once

#include <optional>
#include <array>
#include <cstdint>

namespace esochess {
    struct bitboard {
        enum Turn {White, Black};
        enum BitboardIndex {
            WhitePawn, WhiteKnight, WhiteBishop, WhiteRook, WhiteQueen, WhiteKing,
            BlackPawn, BlackKnight, BlackBishop, BlackRook, BlackQueen, BlackKing
        };

        struct castle_rights {
            bool white_king {};
            bool white_queen {};
            bool black_king {};
            bool black_queen {};
        };

        using bitboard_array = std::array<std::uint64_t, 12>;

        private:

        bitboard_array _pieces_bit_board {};
        Turn _turn {};

        castle_rights _can_castle {};

        std::optional<std::uint64_t> _en_passant_square;

        std::size_t _half_move_clock {};
        std::size_t _full_move_clock {};

        public:

        [[nodiscard]] std::array<std::uint64_t, 12> pieces_bit_board() const;
        [[nodiscard]] Turn turn() const;
        [[nodiscard]] castle_rights can_castle() const;
        [[nodiscard]] std::optional<std::uint64_t> en_passant_square() const;
        [[nodiscard]] std::size_t half_move_clock() const;
        [[nodiscard]] std::size_t full_move_clock() const;
    };
}

#endif
