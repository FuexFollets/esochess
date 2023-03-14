#include <algorithm>
#include <bit>
#include <iostream>
#include <vector>

#include "headers/display.hpp"
#include "headers/bitboard.hpp"

namespace esochess {
    void fancy_display(std::uint64_t square) {
        const int bit_position {std::countl_zero(square)};
        const int x_axis {bit_position % 8};
        const int y_axis {bit_position / 8};

        std::cout << 'a' + x_axis << '1' + y_axis;
    }

    void fancy_display(const bitboard& displayed_bitboard) {
        const bitboard::castle_rights can_castle {displayed_bitboard.can_castle()};

        std::cout
            << "Turn: " << ((displayed_bitboard.turn() == bitboard::White) ? "White" : "Black") << '\n'
            << "Castle rights: "
                << (can_castle.white_king ? "K" : "")
                << (can_castle.white_queen ? "Q" : "")
                << (can_castle.black_king ? "k" : "")
                << (can_castle.black_queen ? "q" : "") << '\n'
            << "Halfmove clock: " << displayed_bitboard.half_move_clock()
            << " Fullmove clock: " << displayed_bitboard.full_move_clock() << '\n';

        std::cout << "En passant square: ";

        if (displayed_bitboard.en_passant_square().has_value()) {
            fancy_display(displayed_bitboard.en_passant_square().value_or(0x0));
        }

        else {
            std::cout << "-";
        }

        std::cout << '\n';

        std::cout << "Bitboard piece position: \n";

        std::vector<std::vector<bitboard::piece>> piece_grid;

        auto grid {displayed_bitboard.export_grid()};

        std::reverse(grid.begin(), grid.end());

        int row_index {8};

        for (const std::array<bitboard::piece, 8>& row: grid) {
            std::cout << row_index-- << ' ';
            for (const bitboard::piece& chess_piece: row) {
                std::cout << chess_piece.symbol << ' ';
            }

            std::cout << '\n';
        }

        std::cout << "  ";
        for (char column_name {'a'}; column_name <= 'h'; ++column_name) {
            std::cout << column_name << ' ';
        }

        std::cout << '\n';
    }
}
