#ifndef ESOCHESS_MOVE_GENERATION_HPP
#define ESOCHESS_MOVE_GENERATION_HPP
#pragma once

#include <optional>

#include "bitboard.hpp"

namespace esochess {
    void add_pawn_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext);
    void add_pawn_en_passant_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext);
    void add_pawn_promotion_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext);

    void add_king_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext);
    void add_king_castle_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext);

    void add_rook_bishop_queen_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext);
    void add_knight_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext);
    void add_bishop_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext,
                          const bitboard::cordinate& at_cordinate);
    void add_rook_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext,
                        const bitboard::cordinate& at_cordinate);
    void add_queen_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext,
                         const bitboard::cordinate& at_cordinate);

    void bitor_add_controlled_squares(
        std::optional<bitboard::bit_representation>& controlled_squares_bits,
        const bitboard::bit_representation& bit_mask);

    void add_controlled_squares_to_bitboard(bitboard& bitboard_ext,
                                            const bitboard::bit_representation& controlled_squares,
                                            bitboard::Turn turn);
} // namespace esochess

#endif
