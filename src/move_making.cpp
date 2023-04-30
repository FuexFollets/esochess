#include "headers/bitboard.hpp"

namespace esochess {
    bitboard& bitboard::make_move(const move_normal& move) {
        const piece piece_moved {piece_at_square(move.start)};
        const piece piece_at_square_moved_to {piece_at_square(move.end)};

        if (piece_at_square_moved_to != pieces::empty_piece) {
            remove_piece_at_square(move.end, piece_at_square_moved_to);
        }

        xor_piece(move.start | move.end, piece_moved);

        return *this;
    }

    bitboard& bitboard::make_move(const move_en_passant& move) {
        const cordinate square_taken_cordinate {move.square_taken.to_cordinate()};
        const Turn opponents_turn {move.square_taken.captureable_piece_color};
        const Turn turn {opposite_turn(opponents_turn)};
        const piece piece_taken {pieces::from_type_and_turn(PieceType::Pawn, opponents_turn)};
        const piece piece_moved {pieces::from_type_and_turn(PieceType::Pawn, turn)};

        const cordinate cordinate_moved_from {square_taken_cordinate.in_direction(
            (move.en_passant_direction == Direction::NorthWest ||
             move.en_passant_direction == Direction::SouthWest)
                ? Direction::East
                : Direction::West,
            1)};

        remove_piece_at_square(square_taken_cordinate, piece_taken);

        xor_piece(cordinate_moved_from.to_bit_representation() |
             square_taken_cordinate.to_bit_representation(), piece_moved);

        return *this;
    }

    bitboard& bitboard::make_move(const move_castle& move) {
        if (move.turn == Turn::White) {
            _bitboards.at(pieces::white_king.bitboard_index) =
                move.castle_type == CastleType::KingSide ? cordinate {"g1"}.to_bit_representation()
                                                         : cordinate {"c1"}.to_bit_representation();

            if (move.castle_type == CastleType::KingSide) {
                xor_piece(cordinate {"h1"}.to_bit_representation() |
                     cordinate {"f1"}.to_bit_representation(), pieces::white_rook);
            }

            else {
                xor_piece(cordinate {"a1"}.to_bit_representation() |
                     cordinate {"d1"}.to_bit_representation(), pieces::white_rook);
            }
        }

        if (move.turn == Turn::Black) {
            _bitboards.at(pieces::black_king.bitboard_index) =
                move.castle_type == CastleType::KingSide ? cordinate {"g8"}.to_bit_representation()
                                                         : cordinate {"c8"}.to_bit_representation();

            if (move.castle_type == CastleType::KingSide) {
                xor_piece(cordinate {"h8"}.to_bit_representation() |
                     cordinate {"f8"}.to_bit_representation(), pieces::black_rook);
            }

            else {
                xor_piece(cordinate {"a8"}.to_bit_representation() |
                     cordinate {"d8"}.to_bit_representation(), pieces::black_rook);
            }
        }

        return *this;
    }

    bitboard& bitboard::make_move(const move_promotion& move) {
        const piece piece_moved {pieces::from_type_and_turn(PieceType::Pawn, _turn)};
        const piece piece_promoted_to {pieces::from_type_and_turn(move.promotion_type, _turn)};
        const cordinate cordinate_moved_to {
            cordinate {move.start}.in_direction(move.promotion_direction, 1)};

        remove_piece_at_square(move.start, piece_moved);

        if (move.promotion_direction == Direction::North ||
            move.promotion_direction == Direction::South) {
            add_piece_at_square(cordinate_moved_to, piece_promoted_to);
        }

        else { // diagonal promotion
            remove_piece_at_square(cordinate_moved_to);
            add_piece_at_square(cordinate_moved_to, piece_promoted_to);
        }

        return *this;
    }
} // namespace esochess
