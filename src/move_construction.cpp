#include "headers/bitboard.hpp"

namespace esochess {
    bitboard::move_normal::move_normal(bitboard::bit_representation bit_from, bitboard::bit_representation bit_to) :
        start {bit_from}, end {bit_to} {}

    bitboard::move_en_passant::move_en_passant(bitboard::en_passant_square square_taken, Direction from_direction) :
        square_taken {square_taken}, en_passant_direction {from_direction} {}

    bitboard::move_castle::move_castle(bitboard::Turn turn, bitboard::CastleType castle_type) :
        turn {turn}, castle_type {castle_type} {}

    bitboard::move_promotion::move_promotion(bit_representation bit_from, PieceType promotion_type, Direction promotion_direction) :
         promotion_direction {promotion_direction}, start {bit_from}, promotion_type {promotion_type} {}
}
