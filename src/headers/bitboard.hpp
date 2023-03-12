#ifndef ESOCHESS_BITBOARD_HPP
#define ESOCHESS_BITBOARD_HPP
#include <string>
#include <vector>
#pragma once

#include <cstddef>
#include <optional>
#include <array>
#include <cstdint>

namespace esochess {
    struct bitboard {
        enum Turn {White, Black};

        struct piece {
            std::size_t index;
            char symbol;
        };

        struct pieces {
            constexpr static piece
                white_pawn {0, 'P'},
                white_knight {1, 'N'},
                white_bishop {2, 'B'},
                white_rook {3, 'R'},
                white_queen {4, 'Q'},
                white_king {5, 'K'},
                black_pawn {6, 'p'},
                black_knight {7, 'n'},
                black_bishop {8, 'b'},
                black_rook {9, 'r'},
                black_queen {10, 'q'},
                black_king {11, 'K'};

            constexpr static std::array<piece, 12> all {
                white_pawn, white_knight, white_bishop, white_rook, white_queen, white_king,
                black_pawn, black_knight, black_bishop, black_rook, black_queen, black_king
            };
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

        [[nodiscard]] std::string export_fen() const;
        [[nodiscard]] std::vector<std::vector<piece>> export_grid() const;
    };
}

#endif
