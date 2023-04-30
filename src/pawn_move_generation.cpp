#include <vector>

#include "headers/bitboard.hpp"
#include "headers/move_generation.hpp"

namespace esochess {
    void add_pawn_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext) {
        using Direction = bitboard::Direction;

        const bitboard::Turn turn {board.turn()};
        const bitboard::Turn opposite_turn {bitboard::opposite_turn(turn)};

        const bitboard::bit_representation pawns_at_turn {board.bitboards().at(
            turn == bitboard::Turn::White ? bitboard::pieces::white_pawn.bitboard_index
                                          : bitboard::pieces::black_pawn.bitboard_index)};

        const std::vector<bitboard::cordinate> pawn_cordinates {
            bitboard::cordinate_from_bit_representation(pawns_at_turn)};

        for (const bitboard::cordinate& pawn_cordinate: pawn_cordinates) {
            if ( // Promotion moves
                (pawn_cordinate.pos_y() == 6 && turn == bitboard::Turn::White) ||
                (pawn_cordinate.pos_y() == 1 && turn == bitboard::Turn::Black)) {
                continue;
            }

            if (turn == bitboard::Turn::White) {
                if (board.color_at_square(pawn_cordinate.in_direction(Direction::North)) ==
                    bitboard::Turn::None) { // Normal moves
                    moves_listing_ext.normal_moves.emplace_back(
                        pawn_cordinate.to_bit_representation(),
                        pawn_cordinate.in_direction(Direction::North).to_bit_representation());

                    if (pawn_cordinate.pos_y() == bitboard::white_pawn_starting_rank &&
                        board.color_at_square(pawn_cordinate.in_direction(Direction::North)) == bitboard::Turn::None) {
                        moves_listing_ext.normal_moves.emplace_back(
                            pawn_cordinate.to_bit_representation(),
                            pawn_cordinate.in_direction(Direction::North, 2).to_bit_representation());
                    }
                }

                for (const bitboard::cordinate& capture_square: // Capture moves
                     {pawn_cordinate.in_direction(Direction::NorthEast), pawn_cordinate.in_direction(Direction::NorthWest, 1)}) {
                    if (bitboard::in_bounds(capture_square) &&
                        board.color_at_square(capture_square) == opposite_turn) {
                        moves_listing_ext.normal_moves.emplace_back(
                            pawn_cordinate.to_bit_representation(),
                            capture_square.to_bit_representation());

                        add_controlled_squares_to_bitboard(
                            board, capture_square.to_bit_representation(), turn);
                    }
                }
            }

            else {
                if (board.color_at_square(pawn_cordinate.in_direction(Direction::South, 1)) ==
                    bitboard::Turn::None) { // Normal moves
                    moves_listing_ext.normal_moves.emplace_back(
                        pawn_cordinate.to_bit_representation(),
                        pawn_cordinate.in_direction(Direction::South, 1).to_bit_representation());

                    if (pawn_cordinate.pos_y() == bitboard::black_pawn_starting_rank &&
                        board.color_at_square(pawn_cordinate.in_direction(Direction::South, 2)) == bitboard::Turn::None) {
                        moves_listing_ext.normal_moves.emplace_back(
                            pawn_cordinate.to_bit_representation(),
                            pawn_cordinate.in_direction(Direction::South, 2).to_bit_representation());
                    }
                }

                for (const bitboard::cordinate& capture_square: // Capture moves
                     {pawn_cordinate.in_direction(Direction::SouthEast, 1), pawn_cordinate.in_direction(Direction::SouthWest, 1)}) {
                    if (bitboard::in_bounds(capture_square) &&
                        board.color_at_square(capture_square) == opposite_turn) {
                        moves_listing_ext.normal_moves.emplace_back(
                            pawn_cordinate.to_bit_representation(),
                            capture_square.to_bit_representation());

                        add_controlled_squares_to_bitboard(
                            board, capture_square.to_bit_representation(), turn);
                    }
                }
            }
        }

        add_pawn_en_passant_moves(board, moves_listing_ext);
        add_pawn_promotion_moves(board, moves_listing_ext);
    }

    void add_pawn_en_passant_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext) {
        using Direction = bitboard::Direction;

        if (!board.en_passant().has_value()) { // No en passant move possible
            return;
        }

        const bitboard::Turn turn {board.turn()};
        const bitboard::Turn opposite_turn {bitboard::opposite_turn(turn)};
        const bitboard::en_passant_square en_passant_square {
            board.en_passant().value_or(bitboard::en_passant_square {})};
        const bitboard::cordinate en_passant_cordinate {en_passant_square.to_cordinate()};

        if (board.turn() == bitboard::Turn::White &&
            (bitboard::in_bounds(en_passant_cordinate.in_direction(Direction::SouthEast, 1))) &&
            (board.piece_at_square(en_passant_cordinate.in_direction(Direction::SouthEast, 1)) ==
             bitboard::pieces::white_pawn) &&
            (board.color_at_square(en_passant_cordinate.in_direction(Direction::SouthEast, 1)) == opposite_turn)) {
            moves_listing_ext.en_passant_moves.emplace_back(en_passant_square,
                                                            bitboard::Direction::NorthWest);
        }

        if (board.turn() == bitboard::Turn::White &&
            (bitboard::in_bounds(en_passant_cordinate.in_direction(Direction::SouthWest, 1))) &&
            (board.piece_at_square(en_passant_cordinate.in_direction(Direction::SouthWest, 1)) ==
             bitboard::pieces::white_pawn) &&
            (board.color_at_square(en_passant_cordinate.in_direction(Direction::SouthWest, 1)) == opposite_turn)) {
            moves_listing_ext.en_passant_moves.emplace_back(en_passant_square,
                                                            bitboard::Direction::NorthEast);
        }

        if (board.turn() == bitboard::Turn::Black &&
            (bitboard::in_bounds(en_passant_cordinate.in_direction(Direction::NorthEast, 1))) &&
            (board.piece_at_square(en_passant_cordinate.in_direction(Direction::NorthEast, 1)) ==
             bitboard::pieces::black_pawn) &&
            (board.color_at_square(en_passant_cordinate.in_direction(Direction::NorthEast, 1)) == opposite_turn)) {
            moves_listing_ext.en_passant_moves.emplace_back(en_passant_square,
                                                            bitboard::Direction::SouthWest);
        }

        if (board.turn() == bitboard::Turn::Black &&
            (bitboard::in_bounds(en_passant_cordinate.in_direction(Direction::NorthWest, 1))) &&
            (board.piece_at_square(en_passant_cordinate.in_direction(Direction::NorthWest, 1)) ==
             bitboard::pieces::black_pawn) &&
            (board.color_at_square(en_passant_cordinate.in_direction(Direction::NorthWest, 1)) == opposite_turn)) {
            moves_listing_ext.en_passant_moves.emplace_back(en_passant_square,
                                                            bitboard::Direction::SouthEast);
        }
    }

    void add_pawn_promotion_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext) {
        using Direction = bitboard::Direction;

        static const bitboard::bit_representation seventh_rank_bits {[]() {
            bitboard::bit_representation bits {};

            for (int file {0}; file < 8; file++) {
                bits |= bitboard::cordinate {file, 6}.to_bit_representation();
            }

            return bits;
        }()};

        static const bitboard::bit_representation second_rank_bits {[]() {
            bitboard::bit_representation bits {};

            for (int file {0}; file < 8; file++) {
                bits |= bitboard::cordinate {file, 1}.to_bit_representation();
            }

            return bits;
        }()};

        const bitboard::Turn turn {board.turn()};
        const bitboard::Turn opposite_turn {bitboard::opposite_turn(turn)};

        if ((board.turn() == bitboard::Turn::White &&
             (board.bitboards().at(bitboard::pieces::white_pawn.bitboard_index) &
              seventh_rank_bits) == 0) ||
            (board.turn() == bitboard::Turn::Black &&
             (board.bitboards().at(bitboard::pieces::black_pawn.bitboard_index) &
              second_rank_bits) == 0)) {
            return;
        }

        if (turn == bitboard::Turn::White) {
            for (const bitboard::cordinate& pawn_cordinate:
                 bitboard::cordinate_from_bit_representation(
                     board.bitboards().at(bitboard::pieces::white_pawn.bitboard_index) &
                     seventh_rank_bits)) {
                if (board.color_at_square(pawn_cordinate.in_direction(Direction::North, 1)) == bitboard::Turn::None) {
                    moves_listing_ext.promotion_moves.emplace_back(
                        pawn_cordinate.to_bit_representation(), bitboard::PieceType::AnyPromotion,
                        bitboard::Direction::North);
                }

                if (board.color_at_square(pawn_cordinate.in_direction(Direction::NorthWest, 1)) == opposite_turn) {
                    moves_listing_ext.promotion_moves.emplace_back(
                        pawn_cordinate.to_bit_representation(), bitboard::PieceType::AnyPromotion,
                        bitboard::Direction::NorthWest);
                    add_controlled_squares_to_bitboard(
                        board, pawn_cordinate.in_direction(Direction::NorthWest, 1).to_bit_representation(), turn);
                }

                if (board.color_at_square(pawn_cordinate.in_direction(Direction::NorthEast, 1)) == opposite_turn) {
                    moves_listing_ext.promotion_moves.emplace_back(
                        pawn_cordinate.to_bit_representation(), bitboard::PieceType::AnyPromotion,
                        bitboard::Direction::NorthEast);
                    add_controlled_squares_to_bitboard(
                        board, pawn_cordinate.in_direction(Direction::NorthEast, 1).to_bit_representation(), turn);
                }
            }
        }

        if (turn == bitboard::Turn::Black) {
            for (const bitboard::cordinate& pawn_cordinate:
                 bitboard::cordinate_from_bit_representation(
                     board.bitboards().at(bitboard::pieces::black_pawn.bitboard_index) &
                     second_rank_bits)) {
                if (board.color_at_square(pawn_cordinate.in_direction(Direction::South, 1)) == bitboard::Turn::None) {
                    moves_listing_ext.promotion_moves.emplace_back(
                        pawn_cordinate.to_bit_representation(), bitboard::PieceType::AnyPromotion,
                        bitboard::Direction::South);
                }

                if (board.color_at_square(pawn_cordinate.in_direction(Direction::SouthWest, 1)) == opposite_turn) {
                    moves_listing_ext.promotion_moves.emplace_back(
                        pawn_cordinate.to_bit_representation(), bitboard::PieceType::AnyPromotion,
                        bitboard::Direction::SouthWest);
                    add_controlled_squares_to_bitboard(
                        board, pawn_cordinate.in_direction(Direction::SouthWest, 1).to_bit_representation(), turn);
                }

                if (board.color_at_square(pawn_cordinate.in_direction(Direction::SouthEast, 1)) == opposite_turn) {
                    moves_listing_ext.promotion_moves.emplace_back(
                        pawn_cordinate.to_bit_representation(), bitboard::PieceType::AnyPromotion,
                        bitboard::Direction::SouthEast);
                    add_controlled_squares_to_bitboard(
                        board, pawn_cordinate.in_direction(Direction::SouthEast, 1).to_bit_representation(), turn);
                }
            }
        }
    }
} // namespace esochess
