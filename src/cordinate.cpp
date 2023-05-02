#include <bit>
#include <string>

#include "headers/bitboard.hpp"

namespace esochess {
    bitboard::cordinate::cordinate(int pos_x, int pos_y) : _x {pos_x}, _y {pos_y} {
    }

    bitboard::cordinate::cordinate(const std::string& cordinate_string) :
        _x {cordinate_string.at(0) - 'a'}, _y {cordinate_string.at(1) - '1'} {
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

    bitboard::cordinate bitboard::cordinate::in_direction(Direction direction,
                                                          int steps) const noexcept {
        switch (direction) {
            case Direction::North: return cordinate {_x, _y + steps};
            case Direction::NorthEast: return cordinate {_x + steps, _y + steps};
            case Direction::East: return cordinate {_x + steps, _y};
            case Direction::SouthEast: return cordinate {_x + steps, _y - steps};
            case Direction::South: return cordinate {_x, _y - steps};
            case Direction::SouthWest: return cordinate {_x - steps, _y - steps};
            case Direction::West: return cordinate {_x - steps, _y};
            case Direction::NorthWest: return cordinate {_x - steps, _y + steps};
        }

        return cordinate {_x, _y};
    }

    bitboard::cordinate bitboard::cordinate::in_direction(Direction direction) const noexcept {
        return in_direction(direction, 1);
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
} // namespace esochess
