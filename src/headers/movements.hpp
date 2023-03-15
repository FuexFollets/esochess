#ifndef ESOCHESS_MOVEMENTS_HPP
#define ESOCHESS_MOVEMENTS_HPP
#pragma once

#include <cstdint>
#include <functional>
#include <vector>

#include "bitboard.hpp"

namespace esochess {
    bool in_bounds(int x_cordinate, int y_cordinate);
    bool in_bounds(const bitboard::cordinate& maybe_in_bounds);


    std::vector<bitboard::cordinate> movements_from_square(std::uint64_t starting_square,
            bitboard::piece chess_piece);

    std::vector<bitboard::cordinate> movements_from_square_as_pawn(bitboard::Turn color,
            std::uint64_t starting_square);
    std::vector<bitboard::cordinate> movements_from_square_as_knight(std::uint64_t starting_square);
    std::vector<bitboard::cordinate> movements_from_square_as_bishop(std::uint64_t starting_square);
    std::vector<bitboard::cordinate> movements_from_square_as_rook(std::uint64_t starting_square);
    std::vector<bitboard::cordinate> movements_from_square_as_queen(std::uint64_t starting_square);
    std::vector<bitboard::cordinate> movements_from_square_as_king(bitboard::Turn color,
            std::uint64_t starting_square);
}

#endif
