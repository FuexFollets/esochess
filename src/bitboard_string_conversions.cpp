#include <algorithm>
#include <array>
#include <string>
#include <sstream>

#include "headers/bitboard.hpp"

namespace esochess {
    bitboard::bitboard(const std::string& fen_position) {
        std::string fen_board;
        std::string fen_turn;
        std::string fen_castle_rights;
        std::string fen_en_passant;
        std::string fen_halfmove_clock;
        std::string fen_fullmove_number;

        std::stringstream {fen_position} >> fen_board >> fen_turn
            >> fen_castle_rights >> fen_en_passant >> fen_halfmove_clock
            >> fen_fullmove_number;


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
                    const piece corresponding_piece {*std::find_if(bitboard::pieces::all_pieces.begin(),
                            bitboard::pieces::all_pieces.end(),
                            [letter] (bitboard::piece maybe_piece) {
                                return maybe_piece.symbol == letter;})};

                    const bitboard::bit_representation bit_position {cordinate {column, row}.to_bit_representation()};

                    _bitboards.at(corresponding_piece.bitboard_index) |= bit_position;
                    column++;
                }
            }

            row--;
        }

        if (fen_turn == "w") { _turn = Turn::White; }
        else { _turn = Turn::Black; }

        if (fen_castle_rights.contains('K')) { _castle_rights.white_king_side = true; }
        if (fen_castle_rights.contains('Q')) { _castle_rights.white_queen_side = true; }
        if (fen_castle_rights.contains('k')) { _castle_rights.black_king_side = true; }
        if (fen_castle_rights.contains('q')) { _castle_rights.black_queen_side = true; }

        if (fen_en_passant == "-") { _en_passant = {}; }

        else {
            const int en_passant_cordinate_x {fen_en_passant[0] - 'a'};
            const int en_passant_cordinate_y {fen_en_passant[1] - '1'};

            _en_passant = en_passant_square {
                static_cast<std::uint8_t>(en_passant_cordinate_x),
                (en_passant_cordinate_y == 3) ? Turn::White : Turn::Black
            };
        }

        _halfmove_clock = std::atoi(fen_halfmove_clock.c_str());
        _fullmove_number = std::atoi(fen_fullmove_number.c_str());
    }

    std::string bitboard::to_fen() const {
        const bitboard::chess_grid grid {this -> to_grid()};

        std::string fen_position {};

        for (std::size_t row_index {}; row_index < 8; row_index++) {
            const std::array<piece, 8>& row {grid.at(row_index)};

            std::size_t empty_squares {0};

            for (const piece& grid_piece: row) {
                if (grid_piece == pieces::empty_piece) {
                    empty_squares++;
                }

                else {
                    if (empty_squares > 0) {
                        fen_position += std::to_string(empty_squares);
                        empty_squares = 0;
                    }

                    fen_position += grid_piece.to_string();
                }
            }

            if (empty_squares > 0) {
                fen_position += std::to_string(empty_squares);
            }

            if (row_index != 7) {
                fen_position += '/';
            }
        }

        return fen_position;
    }
}
