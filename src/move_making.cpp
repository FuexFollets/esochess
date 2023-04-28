#include "headers/bitboard.hpp"

namespace esochess {
    bitboard& bitboard::make_move(const move_normal& move) {
        const piece piece_moved {piece_at_square(move.start)};
        const piece piece_at_square_moved_to {piece_at_square(move.end)};

        if (piece_at_square_moved_to != pieces::empty_piece) {
            _bitboards.at(piece_at_square_moved_to.bitboard_index) ^= move.end;
        }

        _bitboards.at(piece_moved.bitboard_index) ^= move.start;
        _bitboards.at(piece_moved.bitboard_index) |= move.end;


        return *this;
    }
}

