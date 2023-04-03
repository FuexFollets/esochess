#include <algorithm>
#include <array>
#include <bit>
#include <string>

#include "headers/bitboard.hpp"

namespace esochess {
    std::string bitboard::piece::to_string() const {
        return std::string {symbol};
    }

    bitboard::cordinate::cordinate(int pos_x, int pos_y) :
        _x {pos_x}, _y {pos_y} {}

    bitboard::cordinate::cordinate(bitboard::bit_representation bit_mask) :
        _x {std::countr_zero(bit_mask) % 8},
        _y {std::countr_zero(bit_mask / 8)} {}

    int bitboard::cordinate::pos_x() const {
        return _x;
    }

    int bitboard::cordinate::pos_y() const {
        return _y;
    }

    bitboard::bit_representation bitboard::cordinate::to_bit_representation() const {
        return std::size_t {1} << (63 - (_x + _y * 8));
    }

    std::string bitboard::cordinate::to_string() const {
        return std::to_string(_x) + ", " + std::to_string(_y);
    }

    std::string bitboard::cordinate::to_fancy_string() const {
        return std::string {static_cast<char>('a' + _x)} + std::to_string(_y + 1);
    }

    bitboard::chess_grid bitboard::to_grid() const {
        bitboard::chess_grid grid {};

        for (std::array<piece, 8> row: grid) {
            std::fill(row.begin(), row.end(), pieces::empty_piece);
        }

        for (const bitboard::piece& chess_piece: pieces::all_pieces) {
            const std::vector<bitboard::cordinate> cordinates {
                cordinate_from_bit_representation(_bitboards.at(chess_piece.bitboard_index))};

            for (const bitboard::cordinate& cordinate: cordinates) {
                grid.at(cordinate.pos_y())
                    .at(cordinate.pos_x()) = chess_piece;
            }
        }

        return grid;
    }

    std::array<bitboard::bit_representation, 12> bitboard::bitboards() const {
        return _bitboards;
    }

    bitboard::Turn bitboard::opposite_turn(Turn turn) {
        return (turn == Turn::White) ? Turn::Black : Turn::White;
    }

    std::vector<bitboard::cordinate> bitboard::cordinate_from_bit_representation(bitboard::bit_representation bits) {
        std::vector<bitboard::cordinate> cordinates;

        while (bits != 0x0000'0000) {

            const cordinate cordinate_from_bits {bits};
            cordinates.emplace_back(cordinate_from_bits);
            const std::uint64_t bit_mask {~std::bit_floor(bits)};

            bits = bits & bit_mask;
        }

        return cordinates;
    }
}
