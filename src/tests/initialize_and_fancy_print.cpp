#include <iostream>

#include <headers/bitboard.hpp>

int main() {
    esochess::bitboard board {esochess::bitboard::starting_position_fen};

    std::cout << "Board fancy print:\n" << board.to_fancy_string() << '\n';
}
