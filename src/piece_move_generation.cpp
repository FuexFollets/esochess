#include <array>
#include <numeric>

#include "headers/bitboard.hpp"
#include "headers/move_generation.hpp"

namespace esochess {
    void add_king_moves(bitboard& board, bitboard::moves_listing& moves_listing_ext) {
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
