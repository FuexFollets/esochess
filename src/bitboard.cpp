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
    bitboard::bitboard(bitboard_array assigned_bitboard_array, Turn assigned_turn,
            castle_rights assigned_castle_rights,
            std::optional<std::uint64_t> assigned_en_passant_square,
            std::size_t assigned_half_move_clock, std::size_t assigned_full_move_clock) :
        _pieces_bit_board {assigned_bitboard_array},
        _turn {assigned_turn},
        _can_castle {assigned_castle_rights},
        _en_passant_square {assigned_en_passant_square},
        _half_move_clock {assigned_half_move_clock},
        _full_move_clock {assigned_full_move_clock} {}

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

        std::istringstream fen_position_stream {fen_position};

        while (std::getline(fen_position_stream, rank, '/') && row>= 0) {
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
                                return maybe_piece.symbol == letter;})};

                    const std::uint64_t bit_position {get_bits_from_cordinate(cordinate {column, row})};

                    _pieces_bit_board[corresponding_piece.index] |= bit_position;
                    column++;
                }
            }

            row--;
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

    std::uint64_t bitboard::get_bits_from_cordinate(const cordinate& cord) {
        return std::uint64_t {1} << (63 - (cord.x + cord.y * 8));
    }

    std::array<std::array<bitboard::piece, 8>, 8> bitboard::export_grid() const {
        std::array<std::array<piece, 8>, 8> exported_grid {};

        for (const bitboard::piece piece : bitboard::pieces::all) {
            std::cout << "Piece: " << piece.symbol << '\n';
            const std::uint64_t bits {_pieces_bit_board[piece.index]};

            std::cout << "Bits: " << std::bitset<64>(bits) << '\n';

            std::vector<bitboard::cordinate> cordinates {get_cordinates_from_bits(bits)};

            for (const auto [x_cordinate, y_cordinate] : cordinates) {
                std::cout << "X: " << x_cordinate << " Y: " << y_cordinate << '\n';

                exported_grid.at(y_cordinate).at(x_cordinate) = piece;
            }
        }

        return exported_grid;
    }
}
