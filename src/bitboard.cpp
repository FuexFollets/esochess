#include <bit>
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

        while (bits != 0x00000000) {
            const int bit_position {std::countl_zero(bits)};

            const int x_axis {bit_position % 8};
            const int y_axis {bit_position / 8};

            cordinates.emplace_back(cordinate {x_axis, y_axis});

            bits ^= std::bit_ceil(bits);
        }

        return cordinates;
    }

    std::vector<std::vector<bitboard::piece>> bitboard::export_grid() const {
        std::vector<std::vector<bitboard::piece>> exported_grid {};

        for (const bitboard::piece piece : bitboard::pieces::all) {
            const std::uint64_t bits {_pieces_bit_board[piece.index]};
        }

        return exported_grid;
    }
}
