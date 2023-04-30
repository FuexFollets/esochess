#include <array>
#include <numeric>
#include <utility>
#include <vector>

#include "headers/bitboard.hpp"
#include "headers/move_generation.hpp"

namespace esochess {
    void add_king_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext) {
        const bitboard::Turn turn {board.turn()};
        const bitboard::bit_representation king_bitboard {board.bitboards().at(
            turn == bitboard::Turn::White ? bitboard::pieces::white_king.bitboard_index
                                          : bitboard::pieces::black_king.bitboard_index)};

        if (king_bitboard == 0) { // If king does not exist
            return;
        }

        const bitboard::cordinate king_cordinate {
            bitboard::cordinate_from_bit_representation(king_bitboard).at(0)};
        const bitboard::bit_representation opponent_controlled_squares_bits {
            (turn == bitboard::Turn::White ? board.cached_moves_listing().black_controlled_squares
                                           : board.cached_moves_listing().white_controlled_squares)
                .value_or(0)};

        for (const bitboard::Direction king_direction: bitboard::pieces::all_directions) {
            const bitboard::cordinate cordinate_in_direction {
                king_cordinate.in_direction(king_direction)};
            const bitboard::bit_representation cordinate_in_direction_bits {
                cordinate_in_direction.to_bit_representation()};

            add_controlled_squares_to_bitboard(board, cordinate_in_direction_bits, turn);

            if (bitboard::in_bounds(cordinate_in_direction) &&
                (board.bitboard_bitor_accumulation(turn) & cordinate_in_direction_bits) == 0 &&
                (cordinate_in_direction_bits & opponent_controlled_squares_bits) == 0) {
                moves_listing_ext.normal_moves.emplace_back(king_cordinate.to_bit_representation(),
                                                            cordinate_in_direction_bits);
            }
        }

        add_king_castle_moves(board, moves_listing_ext);
    }

    void add_king_castle_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext) {
        const static std::array<bitboard::cordinate, 2> white_kingside_castle_cordinates {
            {bitboard::cordinate {5, 0}, bitboard::cordinate {6, 0}}
        };

        const static std::array<bitboard::cordinate, 2> white_queenside_castle_cordinates {
            {bitboard::cordinate {2, 0}, bitboard::cordinate {3, 0}}
        };

        const static std::array<bitboard::cordinate, 2> black_kingside_castle_cordinates {
            {bitboard::cordinate {5, 7}, bitboard::cordinate {6, 7}}
        };

        const static std::array<bitboard::cordinate, 2> black_queenside_castle_cordinates {
            {bitboard::cordinate {2, 7}, bitboard::cordinate {3, 7}}
        };

        const static bitboard::bit_representation white_kingside_castle_mask {std::accumulate(
            white_kingside_castle_cordinates.begin(), white_kingside_castle_cordinates.end(),
            bitboard::bit_representation {0},
            [](bitboard::bit_representation acc, const bitboard::cordinate& cordinate) {
                return acc | cordinate.to_bit_representation();
            })};

        const static bitboard::bit_representation white_queenside_castle_mask {std::accumulate(
            white_queenside_castle_cordinates.begin(), white_queenside_castle_cordinates.end(),
            bitboard::bit_representation {0},
            [](bitboard::bit_representation acc, const bitboard::cordinate& cordinate) {
                return acc | cordinate.to_bit_representation();
            })};

        const static bitboard::bit_representation black_kingside_castle_mask {std::accumulate(
            black_kingside_castle_cordinates.begin(), black_kingside_castle_cordinates.end(),
            bitboard::bit_representation {0},
            [](bitboard::bit_representation acc, const bitboard::cordinate& cordinate) {
                return acc | cordinate.to_bit_representation();
            })};

        const static bitboard::bit_representation black_queenside_castle_mask {std::accumulate(
            black_queenside_castle_cordinates.begin(), black_queenside_castle_cordinates.end(),
            bitboard::bit_representation {0},
            [](bitboard::bit_representation acc, const bitboard::cordinate& cordinate) {
                return acc | cordinate.to_bit_representation();
            })};

        const bitboard::Turn turn {board.turn()};
        const bitboard::cached_moves_listing_t cached_moves_listing {board.cached_moves_listing()};

        if (turn == bitboard::Turn::White &&
            (board.castle_rights().white_king_side) && // white king side can castle
            ((cached_moves_listing.black_controlled_squares.value_or(
                  bitboard::bit_representation {0}) &
              white_kingside_castle_mask) == white_kingside_castle_mask) &&
            // None of the kingside castle spaces are controlled by black pieces
            ((board.bitboard_bitor_accumulation(bitboard::Turn::All) &
              white_kingside_castle_mask) == 0)
            // No pieces can occupy kingside castle squares
        ) {
            moves_listing_ext.castle_moves.emplace_back(bitboard::Turn::White,
                                                        bitboard::CastleType::KingSide);
        }

        if (turn == bitboard::Turn::White &&
            (board.castle_rights().white_queen_side) && // white queen side can castle
            ((cached_moves_listing.black_controlled_squares.value_or(
                  bitboard::bit_representation {0}) &
              white_queenside_castle_mask) == white_queenside_castle_mask) &&
            // None of the queenside castle spaces are controlled by black pieces
            ((board.bitboard_bitor_accumulation(bitboard::Turn::All) &
              white_queenside_castle_mask) == 0)
            // No pieces can occupy queenside castle squares
        ) {
            moves_listing_ext.castle_moves.emplace_back(bitboard::Turn::White,
                                                        bitboard::CastleType::QueenSide);
        }

        if (turn == bitboard::Turn::Black &&
            (board.castle_rights().black_king_side) && // black king side can castle
            ((cached_moves_listing.white_controlled_squares.value_or(
                  bitboard::bit_representation {0}) &
              black_kingside_castle_mask) == black_kingside_castle_mask) &&
            // None of the kingside castle spaces are controlled by white pieces
            ((board.bitboard_bitor_accumulation(bitboard::Turn::All) &
              black_kingside_castle_mask) == 0)
            // No pieces can occupy kingside castle squares
        ) {
            moves_listing_ext.castle_moves.emplace_back(bitboard::Turn::Black,
                                                        bitboard::CastleType::KingSide);
        }

        if (turn == bitboard::Turn::Black &&
            (board.castle_rights().black_queen_side) && // black queen side can castle
            ((cached_moves_listing.white_controlled_squares.value_or(
                  bitboard::bit_representation {0}) &
              black_queenside_castle_mask) == black_queenside_castle_mask) &&
            // None of the queenside castle spaces are controlled by white pieces
            ((board.bitboard_bitor_accumulation(bitboard::Turn::All) &
              black_queenside_castle_mask) == 0)
            // No pieces can occupy queenside castle squares
        ) {
            moves_listing_ext.castle_moves.emplace_back(bitboard::Turn::Black,
                                                        bitboard::CastleType::QueenSide);
        }
    }

    void add_rook_bishop_queen_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext) {
        const bitboard::Turn turn {board.turn()};

        for (const bitboard::piece& piece:
             (turn == bitboard::Turn::White ? bitboard::pieces::white_rook_bishop_queen
                                            : bitboard::pieces::black_rook_bishop_queen)) {
            const bitboard::bit_representation piece_bits {
                board.bitboards().at(piece.bitboard_index)};
            const std::vector<bitboard::cordinate> cordinates_for_pieces {
                bitboard::cordinate_from_bit_representation(piece_bits)};

            for (const bitboard::cordinate piece_cordinate: cordinates_for_pieces) {
                switch (piece.piece_type) {
                    case bitboard::PieceType::Bishop:
                        add_bishop_moves(board, moves_listing_ext, piece_cordinate);
                    case bitboard::PieceType::Rook:
                        add_rook_moves(board, moves_listing_ext, piece_cordinate);
                    case bitboard::PieceType::Queen:
                        add_queen_moves(board, moves_listing_ext, piece_cordinate);

                    default: break;
                }
            }
        }
    }

    void add_bishop_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext,
                          const bitboard::cordinate& at_cordinate) {
        const bitboard::Turn turn {board.turn()};
        const bitboard::Turn opponents_turn {bitboard::opposite_turn(turn)};
        const bitboard::bit_representation bishop_cordinate_bits {
            at_cordinate.to_bit_representation()};
        const bitboard::bit_representation current_turns_pieces_positions_bits {
            board.bitboard_bitor_accumulation(turn)};
        const bitboard::bit_representation opponent_piece_positions_bits {
            board.bitboard_bitor_accumulation(opponents_turn)};

        for (const bitboard::Direction& direction: bitboard::pieces::bishop_directions) {
            for (int steps {}; steps < 8; steps++) {
                const bitboard::cordinate cordinate_after_steps {
                    at_cordinate.in_direction(direction, steps)};
                const bitboard::bit_representation cordinate_after_steps_bits {
                    cordinate_after_steps.to_bit_representation()};

                add_controlled_squares_to_bitboard(board, cordinate_after_steps_bits, turn);

                if (!bitboard::in_bounds(cordinate_after_steps) ||
                    (current_turns_pieces_positions_bits & cordinate_after_steps_bits) != 0) {
                    break; // Do not continue searching for moves in this direction
                }

                if ((opponent_piece_positions_bits & cordinate_after_steps_bits) != 0) {
                    moves_listing_ext.normal_moves.emplace_back(bishop_cordinate_bits,
                                                                cordinate_after_steps_bits);
                    break;
                }

                moves_listing_ext.normal_moves.emplace_back(bishop_cordinate_bits,
                                                            cordinate_after_steps_bits);
            }
        }
    }

    void add_rook_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext,
                        const bitboard::cordinate& at_cordinate) {
        const bitboard::Turn turn {board.turn()};
        const bitboard::Turn opponents_turn {bitboard::opposite_turn(turn)};
        const bitboard::bit_representation rook_cordinate_bits {
            at_cordinate.to_bit_representation()};
        const bitboard::bit_representation current_turns_pieces_positions_bits {
            board.bitboard_bitor_accumulation(turn)};
        const bitboard::bit_representation opponent_piece_positions_bits {
            board.bitboard_bitor_accumulation(opponents_turn)};

        for (const bitboard::Direction& direction: bitboard::pieces::rook_directions) {
            for (int steps {}; steps < 8; steps++) {
                const bitboard::cordinate cordinate_after_steps {
                    at_cordinate.in_direction(direction, steps)};
                const bitboard::bit_representation cordinate_after_steps_bits {
                    cordinate_after_steps.to_bit_representation()};

                add_controlled_squares_to_bitboard(board, cordinate_after_steps_bits, turn);

                if (!bitboard::in_bounds(cordinate_after_steps) ||
                    (current_turns_pieces_positions_bits & cordinate_after_steps_bits) != 0) {
                    break; // Do not continue searching for moves in this direction
                }

                if ((opponent_piece_positions_bits & cordinate_after_steps_bits) != 0) {
                    moves_listing_ext.normal_moves.emplace_back(rook_cordinate_bits,
                                                                cordinate_after_steps_bits);

                    break;
                }

                moves_listing_ext.normal_moves.emplace_back(rook_cordinate_bits,
                                                            cordinate_after_steps_bits);
            }
        }
    }

    void add_queen_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext,
                         const bitboard::cordinate& at_cordinate) {
        const bitboard::Turn turn {board.turn()};
        const bitboard::Turn opponents_turn {bitboard::opposite_turn(turn)};
        const bitboard::bit_representation queen_cordinate_bits {
            at_cordinate.to_bit_representation()};
        const bitboard::bit_representation current_turns_pieces_positions_bits {
            board.bitboard_bitor_accumulation(turn)};
        const bitboard::bit_representation opponent_piece_positions_bits {
            board.bitboard_bitor_accumulation(opponents_turn)};

        for (const bitboard::Direction& direction: bitboard::pieces::all_directions) {
            for (int steps {}; steps < 8; steps++) {
                const bitboard::cordinate cordinate_after_steps {
                    at_cordinate.in_direction(direction, steps)};
                const bitboard::bit_representation cordinate_after_steps_bits {
                    cordinate_after_steps.to_bit_representation()};

                add_controlled_squares_to_bitboard(board, cordinate_after_steps_bits, turn);

                if (!bitboard::in_bounds(cordinate_after_steps) ||
                    (current_turns_pieces_positions_bits & cordinate_after_steps_bits) != 0) {
                    break; // Do not continue searching for moves in this direction
                }

                if ((opponent_piece_positions_bits & cordinate_after_steps_bits) != 0) {
                    moves_listing_ext.normal_moves.emplace_back(queen_cordinate_bits,
                                                                cordinate_after_steps_bits);

                    break;
                }

                moves_listing_ext.normal_moves.emplace_back(queen_cordinate_bits,
                                                            cordinate_after_steps_bits);
            }
        }
    }

    void add_knight_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext) {
        using Direction = bitboard::Direction;

        static constexpr std::array<std::pair<int, int>, 8> knight_move_differences {
            {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}}
        };

        const bitboard::Turn turn {board.turn()};
        const std::vector<bitboard::cordinate> knight_positions {
            bitboard::cordinate_from_bit_representation(
                turn == bitboard::Turn::White
                    ? board.bitboards().at(bitboard::pieces::white_knight.bitboard_index)
                    : board.bitboards().at(bitboard::pieces::black_knight.bitboard_index))};

        for (const bitboard::cordinate& knight_cordinate: knight_positions) {
            for (const auto& [x_difference, y_difference]: knight_move_differences) {
                const bitboard::cordinate cordinate_after_move {
                    knight_cordinate.in_direction(Direction::North, x_difference)
                        .in_direction(Direction::East, y_difference)};

                add_controlled_squares_to_bitboard(
                    board, cordinate_after_move.to_bit_representation(), turn);

                if (!bitboard::in_bounds(cordinate_after_move)) {
                    continue;
                }

                const bitboard::bit_representation cordinate_after_move_bits {
                    cordinate_after_move.to_bit_representation()};

                if (board.color_at_square(cordinate_after_move_bits) !=
                    turn) { // The square moved to only
                            // needs to hold a different color piece
                    moves_listing_ext.normal_moves.emplace_back(
                        knight_cordinate.to_bit_representation(), cordinate_after_move_bits);
                }
            }
        }
    }

    void bitor_add_controlled_squares(
        std::optional<bitboard::bit_representation>& controlled_squares_bits,
        const bitboard::bit_representation& bit_mask) {
        if (!controlled_squares_bits.has_value()) {
            controlled_squares_bits = bitboard::bit_representation {};
        }

        controlled_squares_bits.transform(
            [&bit_mask](bitboard::bit_representation bits) { return bits | bit_mask; });
    }

    void add_controlled_squares_to_bitboard(bitboard& bitboard_ext,
                                            const bitboard::bit_representation& controlled_squares,
                                            bitboard::Turn turn) {
        bitor_add_controlled_squares(
            turn == bitboard::Turn::White
                ? bitboard_ext.cached_moves_listing().white_controlled_squares
                : bitboard_ext.cached_moves_listing().black_controlled_squares,
            controlled_squares);
    }
} // namespace esochess
