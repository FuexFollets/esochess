#include <string>

#include "headers/bitboard.hpp"

namespace esochess {
    std::string bitboard::piece::to_string() const {
        return std::string {symbol};
    }

    bitboard::cordinate::cordinate(int pos_x, int pos_y) :
        _x {pos_x}, _y {pos_y} {}
}
