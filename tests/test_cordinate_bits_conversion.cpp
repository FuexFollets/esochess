#include <iostream>
#include <vector>

#include "../src/headers/bitboard.hpp"

int main() {
    const std::vector<esochess::bitboard::cordinate> cordinates = esochess::bitboard::get_cordinates_from_bits(0b00000001'00000001'00000001'00000001'00000001'00000001'00000001'00000001);

    for (const esochess::bitboard::cordinate cordinate : cordinates) {
        const auto [cord_x, cord_y] {cordinate};

        std::cout << '(' << cord_x << ", " << cord_y << ")\n";
    }
}
