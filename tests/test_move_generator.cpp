#include <iostream>
#include <vector>

#include "../src/headers/bitboard.hpp"

int main() {
    esochess::bitboard starting_position {esochess::bitboard::starting_position_fen};

    std::vector<esochess::bitboard::move> maybe_legal_moves {starting_position.all_maybe_legal_moves()};

    for (const esochess::bitboard::move& maybe_legal_move : maybe_legal_moves) {
        std::cout
            << esochess::bitboard::get_cordinates_from_bits(maybe_legal_move.bit_mask_from)[0].to_string()
            << " - "
            << esochess::bitboard::get_cordinates_from_bits(maybe_legal_move.bit_mask_to)[0].to_string() << '\n';
    }
}
