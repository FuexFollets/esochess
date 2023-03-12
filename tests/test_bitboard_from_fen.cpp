#include <iostream>

#include "../src/headers/bitboard.hpp"
#include "../src/headers/display.hpp"

int main() {
    esochess::bitboard starting_position {esochess::bitboard::starting_position_fen};

    esochess::fancy_display(starting_position);
}

