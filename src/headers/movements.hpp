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


    std::vector<bitboard::cordinate> movements_from_square(std::uint64_t starting_square, bitboard::piece chess_piece) {
        std::vector<bitboard::cordinate> to_squares {};


        switch (chess_piece.index) {
        case bitboard::pieces::white_pawn.index: {
            bitboard::cordinate cordinate_at_square {starting_square};

            to_squares.emplace_back(cordinate_at_square.x, cordinate_at_square.y + 1);

            if (cordinate_at_square.y == 1) { // If the pawn is at the starting position
                to_squares.emplace_back(cordinate_at_square.x, cordinate_at_square.y + 2);
            }

            if (cordinate_at_square.y == 4) { // If the pawn is in its associated en passant square
                if (cordinate_at_square.x == 0) {
                    to_squares.emplace_back(cordinate_at_square.x + 1, cordinate_at_square.y + 1);
                }

                else if (cordinate_at_square.x == 7) {
                    to_squares.emplace_back(cordinate_at_square.x - 1, cordinate_at_square.y + 1);
                }

                else {
                    to_squares.emplace_back(cordinate_at_square.x + 1, cordinate_at_square.y + 1);
                    to_squares.emplace_back(cordinate_at_square.x - 1, cordinate_at_square.y + 1);
                }
            }

            return to_squares;
        }

        case bitboard::pieces::black_pawn.index: {
            bitboard::cordinate cordinate_at_square {starting_square};

            to_squares.emplace_back(cordinate_at_square.x, cordinate_at_square.y - 1);

            if (cordinate_at_square.y == 6) { // If the pawn is at the starting position
                to_squares.emplace_back(cordinate_at_square.x, cordinate_at_square.y - 2);
            }

            if (cordinate_at_square.y == 3) { // If the pawn is in its associated en passant square
                if (cordinate_at_square.x == 0) {
                    to_squares.emplace_back(cordinate_at_square.x + 1, cordinate_at_square.y - 1);
                }

                else if (cordinate_at_square.x == 7) {
                    to_squares.emplace_back(cordinate_at_square.x - 1, cordinate_at_square.y - 1);
                }

                else {
                    to_squares.emplace_back(cordinate_at_square.x + 1, cordinate_at_square.y - 1);
                    to_squares.emplace_back(cordinate_at_square.x - 1, cordinate_at_square.y - 1);
                }
            }

            return to_squares;
        }

        case bitboard::pieces::white_knight.index: case bitboard::pieces::black_knight.index: {
            bitboard::cordinate cordinate_at_square {starting_square};

            for (const int x_difference : {-2, -1, 1, 2}) {
                for (const int y_difference : {-2, -1, 1, 2}) {
                    if (
                        x_difference == y_difference || x_difference == -y_difference || // Invalid knight moves
                        !in_bounds(cordinate_at_square.x + x_difference, cordinate_at_square.y + y_difference)
                        ) { continue; }

                    to_squares.emplace_back(cordinate_at_square.x + x_difference, cordinate_at_square.y + y_difference);
                }
            }

            return to_squares;
        }

        case bitboard::pieces::white_bishop.index: case bitboard::pieces::black_bishop.index: {
            bitboard::cordinate cordinate_at_square {starting_square};

            for (const int x_difference_multiple: {-1, 1}) {
                for (const int y_difference_multiple: {-1, 1}) {
                    for (int i = 1; i < 8; ++i) {
                        if (!in_bounds(cordinate_at_square.x + i * x_difference_multiple, cordinate_at_square.y + i * y_difference_multiple))
                            { break; }

                        to_squares.emplace_back(cordinate_at_square.x + i * x_difference_multiple, cordinate_at_square.y + i * y_difference_multiple);
                    }
                }
            }

            return to_squares;
        }

        case bitboard::pieces::white_rook.index: case bitboard::pieces::black_rook.index: {
            bitboard::cordinate cordinate_at_square {starting_square};

            for (const auto& [x_difference_multiple, y_difference_multiple]: {
                std::pair {0, -1}, {0, 1}, {-1, 0}, {1, 0}}) {
                for (int i = 1; i < 8; ++i) {
                    if (!in_bounds(cordinate_at_square.x + i * x_difference_multiple, cordinate_at_square.y + i * y_difference_multiple))
                        { break; }

                    to_squares.emplace_back(cordinate_at_square.x + i * x_difference_multiple, cordinate_at_square.y + i * y_difference_multiple);
                }
            }

            return to_squares;
        }

        case bitboard::pieces::white_queen.index: case bitboard::pieces::black_queen.index: {
            bitboard::cordinate cordinate_at_square {starting_square};

            for (const auto& [x_difference_multiple, y_difference_multiple]: {
                std::pair {0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}}) {
                for (int i = 1; i < 8; ++i) {
                    if (!in_bounds(cordinate_at_square.x + i * x_difference_multiple, cordinate_at_square.y + i * y_difference_multiple))
                        { break; }

                    to_squares.emplace_back(cordinate_at_square.x + i * x_difference_multiple, cordinate_at_square.y + i * y_difference_multiple);
                }
            }

            return to_squares;
        }

        case bitboard::pieces::white_king.index: case bitboard::pieces::black_king.index: {
            bitboard::cordinate cordinate_at_square {starting_square};

            return std::vector<bitboard::cordinate> {
                bitboard::cordinate {cordinate_at_square.x, cordinate_at_square.y - 1},
                bitboard::cordinate {cordinate_at_square.x, cordinate_at_square.y + 1},
                bitboard::cordinate {cordinate_at_square.x - 1, cordinate_at_square.y},
                bitboard::cordinate {cordinate_at_square.x + 1, cordinate_at_square.y},
                bitboard::cordinate {cordinate_at_square.x - 1, cordinate_at_square.y - 1},
                bitboard::cordinate {cordinate_at_square.x - 1, cordinate_at_square.y + 1},
                bitboard::cordinate {cordinate_at_square.x + 1, cordinate_at_square.y - 1},
                bitboard::cordinate {cordinate_at_square.x + 1, cordinate_at_square.y + 1}
            };
        }

            default: break;
        }

        return to_squares;
    }
}

#endif
