#ifndef ESOCHESS_MOVE_GENERATION_HPP
#define ESOCHESS_MOVE_GENERATION_HPP
#pragma once

#include "bitboard.hpp"

namespace esochess {
    bitboard::moves_listing& add_pawn_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext);
    bitboard::moves_listing& add_pawn_en_passant_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext);
    bitboard::moves_listing& add_pawn_promotion_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext);

    bitboard::moves_listing& add_king_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext);
    bitboard::moves_listing& add_king_castle_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext);


    bitboard::moves_listing& add_rook_bishop_queen_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext);
    bitboard::moves_listing& add_knight_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext);
}

#endif
