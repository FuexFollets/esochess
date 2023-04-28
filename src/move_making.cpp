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

    bitboard& bitboard::make_move(const move_en_passant& move) {
        const cordinate square_taken_cordinate {move.square_taken.to_cordinate()};
        const Turn opponents_turn {move.square_taken.captureable_piece_color};
        const Turn turn {opposite_turn(opponents_turn)};
        const piece piece_taken {pieces::from_type_and_turn(PieceType::Pawn, opponents_turn)};
        const piece piece_moved {pieces::from_type_and_turn(PieceType::Pawn, turn)};

        const cordinate cordinate_moved_from {
            square_taken_cordinate.in_direction(
                    (move.en_passant_direction == Direction::NorthWest ||
                     move.en_passant_direction == Direction::SouthWest) ? Direction::East : Direction::West, 1)
        };

        _bitboards.at(piece_taken.bitboard_index) ^= square_taken_cordinate.to_bit_representation();
        _bitboards.at(piece_moved.bitboard_index) ^= (cordinate_moved_from.to_bit_representation() |
                                                      square_taken_cordinate.to_bit_representation());

        return *this;
    }

    bitboard& bitboard::make_move(const move_castle& move) {
        if (move.turn == Turn::White) {
            _bitboards.at(pieces::white_king.bitboard_index) =
                move.castle_type == CastleType::KingSide ? cordinate {"g1"}.to_bit_representation()
                : cordinate {"c1"}.to_bit_representation();

            if (move.castle_type == CastleType::KingSide) {
                _bitboards.at(pieces::white_rook.bitboard_index) ^= cordinate {"h1"}.to_bit_representation();
                _bitboards.at(pieces::white_rook.bitboard_index) = cordinate {"f1"}.to_bit_representation();
            } else {
                _bitboards.at(pieces::white_rook.bitboard_index) ^= cordinate {"a1"}.to_bit_representation();
                _bitboards.at(pieces::white_rook.bitboard_index) = cordinate {"d1"}.to_bit_representation();
            }
        }

        if (move.turn == Turn::Black) {
            _bitboards.at(pieces::white_king.bitboard_index) =
                move.castle_type == CastleType::KingSide ? cordinate {"g8"}.to_bit_representation()
                : cordinate {"c8"}.to_bit_representation();

            if (move.castle_type == CastleType::KingSide) {
                _bitboards.at(pieces::white_rook.bitboard_index) ^= cordinate {"h8"}.to_bit_representation();
                _bitboards.at(pieces::white_rook.bitboard_index) = cordinate {"f8"}.to_bit_representation();
            } else {
                _bitboards.at(pieces::white_rook.bitboard_index) ^= cordinate {"a8"}.to_bit_representation();
                _bitboards.at(pieces::white_rook.bitboard_index) = cordinate {"d8"}.to_bit_representation();
            }
        }

        return *this;
    }
}
