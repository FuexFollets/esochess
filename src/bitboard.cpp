#include <algorithm>
#include <bit>
#include <bitset>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "headers/bitboard.hpp"

namespace esochess {
    bitboard::bitboard(const std::string& fen) {
        std::string fen_position;
        std::string fen_turn;
        std::string fen_castle_rights;
        std::string fen_en_passant;
        std::string fen_halfmove_clock;
        std::string fen_full_move_clock;

        std::stringstream {fen} >> fen_position >> fen_turn >> fen_castle_rights >>
            fen_en_passant >> fen_halfmove_clock >> fen_full_move_clock;

        std::string rank;
        int row {7};

        while (std::getline(std::stringstream {fen_position}, rank, '/') && row>= 0) {
            int column {0};

            for (const char letter: rank) {
                if (std::isdigit(letter) != 0) {
                    const int value {letter - '0'};

                    column += value;
                }

                else {
                    const piece corresponding_piece {*std::find_if(bitboard::pieces::all.begin(),
                            bitboard::pieces::all.end(),
                            [letter] (bitboard::piece maybe_piece) {
                                return maybe_piece.symbol == letter;}
                            )};

                    const std::uint64_t bit_position {1ULL << (63 - column + row * 8)};

                    _pieces_bit_board[corresponding_piece.index] |= bit_position;
                }
            }
        }

        if (fen_turn == "w") { _turn = Turn::White; }
        else { _turn = Turn::Black; }

        if (fen_castle_rights.contains('K')) { _can_castle.white_king = true; }
        if (fen_castle_rights.contains('Q')) { _can_castle.white_queen = true; }
        if (fen_castle_rights.contains('k')) { _can_castle.black_king = true; }
        if (fen_castle_rights.contains('q')) { _can_castle.black_queen = true; }

        if (fen_en_passant == "-") { _en_passant_square = {}; }

        else {
            const int en_passant_cordinate_x {fen_en_passant[0] - 'a'};
            const int en_passant_cordinate_y {fen_en_passant[0] - '1'};

            _en_passant_square = 1ULL << (63 - en_passant_cordinate_x + en_passant_cordinate_y * 8);
        }

        _half_move_clock = std::atoi(fen_halfmove_clock.c_str());
        _full_move_clock = std::atoi(fen_full_move_clock.c_str());
    }

    std::array<std::uint64_t, 12> bitboard::pieces_bit_board() const {
        return _pieces_bit_board;
    }

    bitboard::Turn bitboard::turn() const {
        return _turn;
    }

    bitboard::castle_rights bitboard::can_castle() const {
        return _can_castle;
    }

    std::optional<std::uint64_t> bitboard::en_passant_square() const {
        return _en_passant_square;
    }

    std::size_t bitboard::half_move_clock() const {
        return _half_move_clock;
    }

    std::size_t bitboard::full_move_clock() const {
        return _full_move_clock;
    }

    std::vector<bitboard::cordinate> bitboard::get_cordinates_from_bits(std::uint64_t bits) {
        std::vector<bitboard::cordinate> cordinates;

        while (bits != 0x0000'0000) {
            const int bit_position {std::countl_zero(bits)};

            cordinates.emplace_back(cordinate {bit_position % 8, bit_position / 8});

            const std::uint64_t bit_mask {~std::bit_floor(bits)};

            bits = bits & bit_mask;
        }

        return cordinates;
    }

    std::vector<std::vector<bitboard::piece>> bitboard::export_grid() const {
        std::vector<std::vector<bitboard::piece>> exported_grid {};

        for (const bitboard::piece piece : bitboard::pieces::all) {
            const std::uint64_t bits {_pieces_bit_board[piece.index]};

            std::vector<bitboard::cordinate> cordinates {get_cordinates_from_bits(bits)};

            for (const auto [x_cordinate, y_cordinate]: cordinates) {
                exported_grid[x_cordinate][y_cordinate] = piece;
            }
        }

        return exported_grid;
    }
}
