#include <bit>

#include "headers/bitboard.hpp"


namespace esochess {
    bitboard::cordinate::cordinate(int pos_x, int pos_y) : _x {pos_x}, _y {pos_y} {
    }

    bitboard::cordinate::cordinate(bitboard::bit_representation bit_mask) :
        _x {std::countr_zero(bit_mask) % 8}, _y {std::countr_zero(bit_mask / 8)} {
    }

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
}
