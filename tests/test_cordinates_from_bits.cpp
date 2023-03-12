#include <iostream>
#include <vector>

#include "../src/headers/bitboard.hpp"

int main() {
    const std::vector<esochess::bitboard::cordinate> cordinates = esochess::bitboard::get_cordinates_from_bits(0b1000000010000000100000001000000010000000100000001000000010000000);

    for (const esochess::bitboard::cordinate cordinate : cordinates) {
        const auto [cord_x, cord_y] {cordinate};

        std::cout << '(' << cord_x << ", " << cord_y << ")\n";
    }
}
