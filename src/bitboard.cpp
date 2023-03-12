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
}
