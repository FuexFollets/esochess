#include <array>
#include <numeric>

#include "headers/bitboard.hpp"
#include "headers/move_generation.hpp"

namespace esochess {
    void add_king_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext) {
        constexpr static std::array<bitboard::Direction, 8> all_directions {
            bitboard::Direction::North,
            bitboard::Direction::NorthEast,
            bitboard::Direction::East,
            bitboard::Direction::SouthEast,
            bitboard::Direction::South,
            bitboard::Direction::SouthWest,
            bitboard::Direction::West,
            bitboard::Direction::NorthWest
        };

        const bitboard::Turn turn {board.turn()};
        const bitboard::bit_representation king_bitboard {
            board
                .bitboards()
                .at(turn == bitboard::Turn::White ? bitboard::pieces::white_king.bitboard_index
                                                  : bitboard::pieces::black_king.bitboard_index)
        };

        if (king_bitboard == 0) { // If king does not exist
            return;
        }

        const bitboard::cordinate king_cordinate {bitboard::cordinate_from_bit_representation(king_bitboard).at(0)};
        const bitboard::bit_representation opponent_controlled_squares_bits {
            (turn == bitboard::Turn::White ? board.cached_moves_listing().black_controlled_squares
                                           : board.cached_moves_listing().white_controlled_squares).value_or(0)
        };

        for (const bitboard::Direction king_direction: all_directions) {
            const bitboard::cordinate cordinate_in_direction {king_cordinate.in_direction(king_direction, 1)};
            const bitboard::bit_representation cordinate_in_direction_bits {cordinate_in_direction.to_bit_representation()};

            if (
                bitboard::in_bounds(cordinate_in_direction) &&
                (board.bitboard_bitor_accumulation(turn) & cordinate_in_direction_bits) == 0 &&
                (cordinate_in_direction_bits & opponent_controlled_squares_bits) == 0
            ) {
                moves_listing_ext.normal_moves.emplace_back(
                        king_cordinate.to_bit_representation(),
                        cordinate_in_direction_bits);
            }

        }

        add_king_castle_moves(board, moves_listing_ext);
    }

    void add_king_castle_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext) {
        const static std::array<bitboard::cordinate, 2> white_kingside_castle_cordinates {{
            bitboard::cordinate {5, 0},
            bitboard::cordinate {6, 0}
        }};

        const static std::array<bitboard::cordinate, 2> white_queenside_castle_cordinates {{
            bitboard::cordinate {2, 0},
            bitboard::cordinate {3, 0}
        }};

        const static std::array<bitboard::cordinate, 2> black_kingside_castle_cordinates {{
            bitboard::cordinate {5, 7},
            bitboard::cordinate {6, 7}
        }};

        const static std::array<bitboard::cordinate, 2> black_queenside_castle_cordinates {{
            bitboard::cordinate {2, 7},
            bitboard::cordinate {3, 7}
        }};

        const static bitboard::bit_representation white_kingside_castle_mask {
            std::accumulate(white_kingside_castle_cordinates.begin(),
                            white_kingside_castle_cordinates.end(),
                            bitboard::bit_representation {0},
                            [](bitboard::bit_representation acc, const bitboard::cordinate& cordinate) {
                                return acc | cordinate.to_bit_representation();
                            })
        };

        const static bitboard::bit_representation white_queenside_castle_mask {
            std::accumulate(white_queenside_castle_cordinates.begin(),
                            white_queenside_castle_cordinates.end(),
                            bitboard::bit_representation {0},
                            [](bitboard::bit_representation acc, const bitboard::cordinate& cordinate) {
                                return acc | cordinate.to_bit_representation();
                            })
        };

        const static bitboard::bit_representation black_kingside_castle_mask {
            std::accumulate(black_kingside_castle_cordinates.begin(),
                            black_kingside_castle_cordinates.end(),
                            bitboard::bit_representation {0},
                            [](bitboard::bit_representation acc, const bitboard::cordinate& cordinate) {
                                return acc | cordinate.to_bit_representation();
                            })
        };

        const static bitboard::bit_representation black_queenside_castle_mask {
            std::accumulate(black_queenside_castle_cordinates.begin(),
                            black_queenside_castle_cordinates.end(),
                            bitboard::bit_representation {0},
                            [](bitboard::bit_representation acc, const bitboard::cordinate& cordinate) {
                                return acc | cordinate.to_bit_representation();
                            })
        };

        const bitboard::Turn turn {board.turn()};
        const bitboard::cached_moves_listing_t cached_moves_listing {board.cached_moves_listing()};

        if (turn == bitboard::Turn::White &&
            (board.castle_rights().white_king_side) && // white king side can castle
            ((cached_moves_listing.black_controlled_squares.value_or(bitboard::bit_representation {0})
              & white_kingside_castle_mask) == white_kingside_castle_mask) &&
            // None of the kingside castle spaces are controlled by black pieces
            ((board.bitboard_bitor_accumulation(bitboard::Turn::All) & white_kingside_castle_mask) == 0)
            // No pieces can occupy kingside castle squares
        ) {
            moves_listing_ext.castle_moves.emplace_back(bitboard::Turn::White, bitboard::CastleType::KingSide);
        }

        if (turn == bitboard::Turn::White &&
            (board.castle_rights().white_queen_side) && // white queen side can castle
            ((cached_moves_listing.black_controlled_squares.value_or(bitboard::bit_representation {0})
              & white_queenside_castle_mask) == white_queenside_castle_mask) &&
            // None of the queenside castle spaces are controlled by black pieces
            ((board.bitboard_bitor_accumulation(bitboard::Turn::All) & white_queenside_castle_mask) == 0)
            // No pieces can occupy queenside castle squares
        ) {
            moves_listing_ext.castle_moves.emplace_back(bitboard::Turn::White, bitboard::CastleType::QueenSide);
        }

        if (turn == bitboard::Turn::Black &&
            (board.castle_rights().black_king_side) && // black king side can castle
            ((cached_moves_listing.white_controlled_squares.value_or(bitboard::bit_representation {0})
              & black_kingside_castle_mask) == black_kingside_castle_mask) &&
            // None of the kingside castle spaces are controlled by white pieces
            ((board.bitboard_bitor_accumulation(bitboard::Turn::All) & black_kingside_castle_mask) == 0)
            // No pieces can occupy kingside castle squares
        ) {
            moves_listing_ext.castle_moves.emplace_back(bitboard::Turn::Black, bitboard::CastleType::KingSide);
        }

        if (turn == bitboard::Turn::Black &&
            (board.castle_rights().black_queen_side) && // black queen side can castle
            ((cached_moves_listing.white_controlled_squares.value_or(bitboard::bit_representation {0})
              & black_queenside_castle_mask) == black_queenside_castle_mask) &&
            // None of the queenside castle spaces are controlled by white pieces
            ((board.bitboard_bitor_accumulation(bitboard::Turn::All) & black_queenside_castle_mask) == 0)
            // No pieces can occupy queenside castle squares
        ) {
            moves_listing_ext.castle_moves.emplace_back(bitboard::Turn::Black, bitboard::CastleType::QueenSide);
        }
    }
}
