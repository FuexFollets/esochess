#include <vector>

#include "headers/bitboard.hpp"
#include "headers/move_generation.hpp"

namespace esochess {
    void add_pawn_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext) {
        const bitboard::Turn turn {board.turn()};
        const bitboard::Turn opposite_turn {bitboard::opposite_turn(turn)};

        const bitboard::bit_representation pawns_at_turn {
            board.bitboards()
                .at(turn == bitboard::Turn::White ?
                bitboard::pieces::white_pawn.bitboard_index :
                bitboard::pieces::black_pawn.bitboard_index)};

        const std::vector<bitboard::cordinate> pawn_cordinates {bitboard::cordinate_from_bit_representation(pawns_at_turn)};

        for (const bitboard::cordinate& pawn_cordinate : pawn_cordinates) {
            if ( // Promotion moves
                (pawn_cordinate.pos_y() == 6 && turn == bitboard::Turn::White) ||
                (pawn_cordinate.pos_y() == 1 && turn == bitboard::Turn::Black)) {
                continue;
            }

            if (turn == bitboard::Turn::White) {
                if (board.color_at_square(pawn_cordinate.north(1)) == bitboard::Turn::None) {
                    moves_listing_ext.normal_moves.emplace_back(
                        pawn_cordinate.to_bit_representation(),
                        pawn_cordinate.north(1).to_bit_representation()
                    );

                    if (pawn_cordinate.pos_y() == bitboard::white_pawn_starting_rank &&
                        board.color_at_square(pawn_cordinate.north(2)) == bitboard::Turn::None) {

                        moves_listing_ext.normal_moves.emplace_back(
                            pawn_cordinate.to_bit_representation(),
                            pawn_cordinate.north(2).to_bit_representation()
                        );
                    }
                }

                for (const bitboard::cordinate& capture_square :
                        {pawn_cordinate.northeast(1), pawn_cordinate.northwest(1)}) {

                    if (bitboard::in_bounds(capture_square) &&
                        board.color_at_square(capture_square) == opposite_turn) {

                        moves_listing_ext.normal_moves.emplace_back(
                            pawn_cordinate.to_bit_representation(),
                            capture_square.to_bit_representation()
                        );
                    }
                }
            }

            else {
                if (board.color_at_square(pawn_cordinate.south(1)) == bitboard::Turn::None) {
                    moves_listing_ext.normal_moves.emplace_back(
                        pawn_cordinate.to_bit_representation(),
                        pawn_cordinate.south(1).to_bit_representation()
                    );

                    if (pawn_cordinate.pos_y() == bitboard::black_pawn_starting_rank &&
                        board.color_at_square(pawn_cordinate.south(2)) == bitboard::Turn::None) {

                        moves_listing_ext.normal_moves.emplace_back(
                            pawn_cordinate.to_bit_representation(),
                            pawn_cordinate.south(2).to_bit_representation()
                        );
                    }
                }

                for (const bitboard::cordinate& capture_square :
                        {pawn_cordinate.southeast(1), pawn_cordinate.southwest(1)}) {

                    if (bitboard::in_bounds(capture_square) &&
                        board.color_at_square(capture_square) == opposite_turn) {

                        moves_listing_ext.normal_moves.emplace_back(
                            pawn_cordinate.to_bit_representation(),
                            capture_square.to_bit_representation()
                        );
                    }
                }
            }
        }

        add_pawn_en_passant_moves(board, moves_listing_ext);
        add_pawn_promotion_moves(board, moves_listing_ext);
    }

    void add_pawn_en_passant_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext) {
        if (!board.en_passant().has_value()) { // No en passant move possible
            return;
        }

        const bitboard::Turn turn {board.turn()};
        const bitboard::Turn opposite_turn {bitboard::opposite_turn(turn)};
        const bitboard::en_passant_square en_passant_square {board.en_passant().value_or(bitboard::en_passant_square {})};
        const bitboard::cordinate en_passant_cordinate {en_passant_square.to_cordinate()};

        if (
            board.turn() == bitboard::Turn::White &&
            (bitboard::in_bounds(en_passant_cordinate.southeast(1))) &&
            (board.piece_at_square(en_passant_cordinate.southeast(1)) == bitboard::pieces::white_pawn) &&
            (board.color_at_square(en_passant_cordinate.southeast(1)) == opposite_turn)
        ) {
            moves_listing_ext.en_passant_moves.emplace_back(
                en_passant_square, bitboard::Direction::NorthWest);
        }

        if (
            board.turn() == bitboard::Turn::White &&
            (bitboard::in_bounds(en_passant_cordinate.southwest(1))) &&
            (board.piece_at_square(en_passant_cordinate.southwest(1)) == bitboard::pieces::white_pawn) &&
            (board.color_at_square(en_passant_cordinate.southwest(1)) == opposite_turn)
        ) {
            moves_listing_ext.en_passant_moves.emplace_back(
                en_passant_square, bitboard::Direction::NorthEast);
        }

        if (
            board.turn() == bitboard::Turn::Black &&
            (bitboard::in_bounds(en_passant_cordinate.northeast(1))) &&
            (board.piece_at_square(en_passant_cordinate.northeast(1)) == bitboard::pieces::black_pawn) &&
            (board.color_at_square(en_passant_cordinate.northeast(1)) == opposite_turn)
        ) {
            moves_listing_ext.en_passant_moves.emplace_back(
                en_passant_square, bitboard::Direction::SouthWest);
        }

        if (
            board.turn() == bitboard::Turn::Black &&
            (bitboard::in_bounds(en_passant_cordinate.northwest(1))) &&
            (board.piece_at_square(en_passant_cordinate.northwest(1)) == bitboard::pieces::black_pawn) &&
            (board.color_at_square(en_passant_cordinate.northwest(1)) == opposite_turn)
        ) {
            moves_listing_ext.en_passant_moves.emplace_back(
                en_passant_square, bitboard::Direction::SouthEast);
        }
    }
}
