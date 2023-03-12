#include <bit>
#include <iostream>
#include <bitset>
#include <vector>

#include "headers/bitboard.hpp"

namespace esochess {
    std::array<std::uint64_t, 12> bitboard::pieces_bit_board() const {
        return _pieces_bit_board;
    }

    bitboard::Turn bitboard::turn() const {
        return _turn;
    }

    bitboard::castle_rights bitboard::can_castle() const {
        return _can_castle;
    }

    std::optional<std::uint64_t> bitboard::en_passant_square() const {
        return _en_passant_square;
    }

    std::size_t bitboard::half_move_clock() const {
        return _half_move_clock;
    }

    std::size_t bitboard::full_move_clock() const {
        return _full_move_clock;
    }

    std::vector<bitboard::cordinate> bitboard::get_cordinates_from_bits(std::uint64_t bits) {
        std::vector<bitboard::cordinate> cordinates;

        while (bits != 0x0000'0000) {
            const int bit_position {std::countl_zero(bits)};

            cordinates.emplace_back(cordinate {bit_position % 8, bit_position / 8});

            const std::uint64_t bit_mask {~std::bit_floor(bits)};

            bits = bits & bit_mask;
        }

        return cordinates;
    }

    std::vector<std::vector<bitboard::piece>> bitboard::export_grid() const {
        std::vector<std::vector<bitboard::piece>> exported_grid {};

        for (const bitboard::piece piece : bitboard::pieces::all) {
            const std::uint64_t bits {_pieces_bit_board[piece.index]};

            std::vector<bitboard::cordinate> cordinates {get_cordinates_from_bits(bits)};

            for (const auto [x_cordinate, y_cordinate]: cordinates) {
                exported_grid[x_cordinate][y_cordinate] = piece;
            }
        }

        return exported_grid;
    }
}
