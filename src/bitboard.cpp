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


}
