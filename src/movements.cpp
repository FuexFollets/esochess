#include <vector>

#include "headers/movements.hpp"

namespace esochess {
    bool in_bounds(int x_cordinate, int y_cordinate) {
        return x_cordinate >= 0 && x_cordinate < 8 && y_cordinate >= 0 && y_cordinate < 8;
    }

    bool in_bounds(bitboard::cordinate cordinate) {
        return in_bounds(cordinate.x, cordinate.y);
    }

    std::vector<bitboard::cordinate> movements_from_square_as_pawn(bitboard::Turn color,
            std::uint64_t starting_square) {
        std::vector<bitboard::cordinate> to_squares {};
        bitboard::cordinate cordinate_at_square {starting_square};

        to_squares.emplace_back(cordinate_at_square.x, cordinate_at_square.y + 1);

        if (color == bitboard::Turn::White) {

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
        }

        else {
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
        }


        return to_squares;
    }

    std::vector<bitboard::cordinate> movements_from_square_as_knight(std::uint64_t starting_square) {
        std::vector<bitboard::cordinate> to_squares {};
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

    std::vector<bitboard::cordinate> movements_from_square_as_bishop(std::uint64_t starting_square) {
        std::vector<bitboard::cordinate> to_squares {};
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

    std::vector<bitboard::cordinate> movements_from_square_as_rook(std::uint64_t starting_square) {
        std::vector<bitboard::cordinate> to_squares {};
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

    std::vector<bitboard::cordinate> movements_from_square_as_queen(std::uint64_t starting_square) {
        std::vector<bitboard::cordinate> to_squares {};
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

    std::vector<bitboard::cordinate> movements_from_square_as_king(bitboard::Turn color,
            std::uint64_t starting_square) {
        bitboard::cordinate cordinate_at_square {starting_square};
        std::vector<bitboard::cordinate> to_squares {};

        for (const auto& [x_difference, y_difference]: {
            std::pair {0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}}) {
            if (!in_bounds(cordinate_at_square.x + x_difference, cordinate_at_square.y + y_difference))
                { continue; }

            to_squares.emplace_back(cordinate_at_square.x + x_difference, cordinate_at_square.y + y_difference);
        }

        if (color == bitboard::Turn::White) {
            if (starting_square == bitboard::get_bits_from_cordinate(bitboard::cordinate {4, 0})) {
                to_squares.emplace_back(6, 0);
                to_squares.emplace_back(2, 0);
            }
        } else {
            if (starting_square == bitboard::get_bits_from_cordinate(bitboard::cordinate {4, 7})) {
                to_squares.emplace_back(6, 7);
                to_squares.emplace_back(2, 7);
            }
        }

        return to_squares;
    }

    std::vector<bitboard::cordinate> movements_from_square(std::uint64_t starting_square, bitboard::piece chess_piece) {
        switch (chess_piece.index) {
        case bitboard::pieces::white_pawn.index: {
            return movements_from_square_as_pawn(bitboard::Turn::White, starting_square);
        }

        case bitboard::pieces::black_pawn.index: {
            return movements_from_square_as_pawn(bitboard::Turn::Black, starting_square);
        }

        case bitboard::pieces::white_knight.index: case bitboard::pieces::black_knight.index: {
            return movements_from_square_as_knight(starting_square);
        }

        case bitboard::pieces::white_bishop.index: case bitboard::pieces::black_bishop.index: {
            return movements_from_square_as_bishop(starting_square);
        }

        case bitboard::pieces::white_rook.index: case bitboard::pieces::black_rook.index: {
            return movements_from_square_as_rook(starting_square);
        }

        case bitboard::pieces::white_queen.index: case bitboard::pieces::black_queen.index: {
            return movements_from_square_as_queen(starting_square);
        }

        case bitboard::pieces::white_king.index: {
            return movements_from_square_as_king(bitboard::Turn::White, starting_square);
        }

        case bitboard::pieces::black_king.index: {
            return movements_from_square_as_king(bitboard::Turn::Black, starting_square);
        }

            default: break;
        }

        return std::vector<bitboard::cordinate> {};
    }
}