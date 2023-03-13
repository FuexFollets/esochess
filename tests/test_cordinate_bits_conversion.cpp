#include <bitset>
#include <iostream>
#include <vector>

#include "../src/headers/bitboard.hpp"

int main() {
    const std::vector<esochess::bitboard::cordinate> cordinates = esochess::bitboard::get_cordinates_from_bits(0b00000001'00000001'00000001'00000001'00000001'00000001'00000001'00000001);

    std::cout << "Cordinates:\n";

    for (const esochess::bitboard::cordinate cordinate : cordinates) {
        const auto [cord_x, cord_y] {cordinate};

        std::cout << '(' << cord_x << ", " << cord_y << ")\n";
    }

    esochess::bitboard::cordinate cordinate_0_3 {3, 0};

    std::cout << std::bitset<64>(esochess::bitboard::get_bits_from_cordinate(cordinate_0_3)) << '\n';
}
