#ifndef ESOCHESS_BITBOARD_HPP
#define ESOCHESS_BITBOARD_HPP
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace esochess {
    struct bitboard {
        enum Turn {White, Black, None};

        static constexpr const char* const starting_position_fen {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};

        struct piece {
            std::size_t index {};
            char symbol {};
            Turn color {None};

            bool operator==(const piece&) const = default;
        };

        struct cordinate {
            cordinate() = default;
            cordinate(int assigned_x, int assigned_y);
            explicit cordinate(std::uint64_t bit_mask);

            int x;
            int y;

            [[nodiscard]] std::string to_string() const;
            [[nodiscard]] std::uint64_t to_bits() const;

            bool operator==(const cordinate&) const = default;
        };

        struct pieces {
            constexpr static piece
                white_pawn {0, 'P', Turn::White},
                white_knight {1, 'N', Turn::White},
                white_bishop {2, 'B', Turn::White},
                white_rook {3, 'R', Turn::White},
                white_queen {4, 'Q', Turn::White},
                white_king {5, 'K', Turn::White},
                black_pawn {6, 'p', Turn::Black},
                black_knight {7, 'n', Turn::Black},
                black_bishop {8, 'b', Turn::Black},
                black_rook {9, 'r', Turn::Black},
                black_queen {10, 'q', Turn::Black},
                black_king {11, 'k', Turn::Black};

            constexpr static std::array<piece, 12> all {
                white_pawn, white_knight, white_bishop, white_rook, white_queen, white_king,
                black_pawn, black_knight, black_bishop, black_rook, black_queen, black_king
            };

            constexpr static std::array<piece, 6> all_white_pieces {
                white_pawn, white_knight, white_bishop, white_rook, white_queen, white_king
            };
            constexpr static std::array<piece, 6> all_black_pieces {
                black_pawn, black_knight, black_bishop, black_rook, black_queen, black_king
            };
        };

        struct castle_rights {
            bool white_king {};
            bool white_queen {};
            bool black_king {};
            bool black_queen {};
        };

        struct move {
            piece piece_moved {};
            std::uint64_t bit_mask_from {};
            std::uint64_t bit_mask_to {};

            [[nodiscard]] std::tuple<cordinate, cordinate> to_cordinate() const;

            move() = default;
            move(const piece& assigned_piece_moved, std::uint64_t assigned_bit_mask_from,
                    std::uint64_t assigned_bit_mask_to);
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

        bitboard() = default;
        bitboard(const bitboard& other) = default;

        explicit bitboard(const std::string& fen);

        bitboard(bitboard_array assigned_bitboard_array, Turn assigned_turn,
                castle_rights assigned_castle_rights,
                std::optional<std::uint64_t> assigned_en_passant_square,
                std::size_t assigned_half_move_clock, std::size_t assigned_full_move_clock);

        [[nodiscard]] std::array<std::uint64_t, 12> pieces_bit_board() const;
        [[nodiscard]] Turn turn() const;
        [[nodiscard]] castle_rights can_castle() const;
        [[nodiscard]] std::optional<std::uint64_t> en_passant_square() const;
        [[nodiscard]] std::size_t half_move_clock() const;
        [[nodiscard]] std::size_t full_move_clock() const;

        [[nodiscard]] std::string export_fen() const;
        [[nodiscard]] std::array<std::array<piece, 8>, 8> export_grid() const;

        [[nodiscard]] bool is_legal_position() const;
        [[nodiscard]] std::vector<move> all_maybe_legal_moves() const;
        [[nodiscard]] std::vector<move> all_legal_moves() const;
        [[nodiscard]] Turn piece_color_at_square(std::uint64_t bit_mask) const;
        [[nodiscard]] Turn piece_color_at_square(const cordinate& cord) const;

        void make_move(const move& move_to_make);

        static std::vector<cordinate> get_cordinates_from_bits(std::uint64_t bits);
        static std::uint64_t get_bits_from_cordinate(const cordinate& cord);
        static Turn opposite_turn(Turn turn);
    };
}


#endif
