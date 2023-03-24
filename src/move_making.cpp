#include <algorithm>
#include <vector>

#include "headers/bitboard.hpp"

namespace esochess{
    bitboard& bitboard::make_move(const bitboard::move& move_to_make) {
        _pieces_bit_board.at(move_to_make.piece_moved.index) &= ~move_to_make.bit_mask_from;
        _pieces_bit_board.at(move_to_make.piece_moved.index) &= ~move_to_make.bit_mask_to;

        return *this;
    }


    bitboard::king_can_be_taken_result bitboard::king_can_be_taken(const std::vector<move>& all_moves) const {
        const std::uint64_t opposite_king_bit_mask {
            _turn == Turn::White ? _pieces_bit_board.at(pieces::black_king.index) :
                _pieces_bit_board.at(pieces::white_king.index)
        };

        return bitboard::king_can_be_taken_result {
            std::any_of(all_moves.begin(), all_moves.end(),
                [opposite_king_bit_mask](const bitboard::move& maybe_legal_move) -> bool {
            return (maybe_legal_move.bit_mask_to & opposite_king_bit_mask) != 0;}),

            all_moves};
    }

    bitboard::king_can_be_taken_result bitboard::king_can_be_taken() const {
        return bitboard::king_can_be_taken(all_maybe_legal_moves());
    }

    bitboard::castle_squares_are_controlled_result
        bitboard::castle_squares_are_controlled(const std::vector<move>& all_moves) const {
            static const std::array<std::uint64_t, 2> white_king_castle_squares {
                bitboard::cordinate {0, 5}.to_bits(),
                bitboard::cordinate {0, 6}.to_bits()
            };

            static const std::array<std::uint64_t, 2> white_queen_castle_squares {
                bitboard::cordinate {0, 3}.to_bits(),
                bitboard::cordinate {0, 2}.to_bits()
            };

            static const std::array<std::uint64_t, 2> black_king_castle_squares {
                bitboard::cordinate {7, 5}.to_bits(),
                bitboard::cordinate {7, 6}.to_bits()
            };

            static const std::array<std::uint64_t, 2> black_queen_castle_squares {
                bitboard::cordinate {7, 3}.to_bits(),
                bitboard::cordinate {7, 2}.to_bits()
            };

            castle_squares_are_controlled_result controlled {};

            for (const bitboard::move& maybe_legal_move : all_moves) {
                if (_turn == Turn::White) {
                    if ((maybe_legal_move.bit_mask_to & black_king_castle_squares.at(0)) != 0 ||
                        (maybe_legal_move.bit_mask_to & black_king_castle_squares.at(1)) != 0) {
                        controlled.rights.black_king = true;
                    }

                    if ((maybe_legal_move.bit_mask_to & black_queen_castle_squares.at(0)) != 0 ||
                        (maybe_legal_move.bit_mask_to & black_queen_castle_squares.at(1)) != 0) {
                        controlled.rights.black_queen = true;
                    }
                }

                else if (_turn == Turn::Black) {
                    if ((maybe_legal_move.bit_mask_to & white_king_castle_squares.at(0)) != 0 ||
                        (maybe_legal_move.bit_mask_to & white_king_castle_squares.at(1)) != 0) {
                        controlled.rights.white_king = true;
                    }

                    if ((maybe_legal_move.bit_mask_to & white_queen_castle_squares.at(0)) != 0 ||
                        (maybe_legal_move.bit_mask_to & white_queen_castle_squares.at(1)) != 0) {
                        controlled.rights.white_queen = true;
                    }
                }
            }

            return controlled;
        };

    bitboard::castle_squares_are_controlled_result
        bitboard::castle_squares_are_controlled() const {
        return castle_squares_are_controlled(all_maybe_legal_moves());
    };
}
