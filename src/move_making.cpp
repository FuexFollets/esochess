#include <algorithm>
#include <vector>

#include "headers/bitboard.hpp"

namespace esochess{
    bitboard& bitboard::make_move(const bitboard::move& move_to_make) {
        _pieces_bit_board.at(move_to_make.piece_moved.index) &= ~move_to_make.bit_mask_from;
        _pieces_bit_board.at(move_to_make.piece_moved.index) &= ~move_to_make.bit_mask_to;

        return *this;
    }

    bool bitboard::king_can_be_taken() const {
        const std::vector<bitboard::move> all_moves {all_maybe_legal_moves()};

        const std::uint64_t opposite_king_bit_mask {
            _turn == Turn::White ? _pieces_bit_board.at(pieces::black_king.index) :
                _pieces_bit_board.at(pieces::white_king.index)
        };

        return std::any_of(all_moves.begin(), all_moves.end(), 
                [opposite_king_bit_mask](const bitboard::move& maybe_legal_move) -> bool {
            return (maybe_legal_move.bit_mask_to & opposite_king_bit_mask) != 0;
        });
    }
}

