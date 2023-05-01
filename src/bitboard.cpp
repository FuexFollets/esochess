#include <algorithm>
#include <array>
#include <bit>
#include <string>

#include "headers/bitboard.hpp"

namespace esochess {
    std::string bitboard::piece::to_string() const {
        return std::string {symbol};
    }

    bitboard::chess_grid bitboard::to_grid() const {
        bitboard::chess_grid grid {};

        for (std::array<piece, 8> row: grid) {
            std::fill(row.begin(), row.end(), pieces::empty_piece);
        }

        for (const bitboard::piece& chess_piece: pieces::all_pieces) {
            const std::vector<bitboard::cordinate> cordinates {
                cordinate_from_bit_representation(_bitboards.at(chess_piece.bitboard_index))};

            for (const bitboard::cordinate& cordinate: cordinates) {
                grid.at(cordinate.pos_y()).at(cordinate.pos_x()) = chess_piece;
            }
        }

        return grid;
    }

    std::array<bitboard::bit_representation, 12> bitboard::bitboards() const {
        return _bitboards;
    }

    bitboard::Turn bitboard::opposite_turn(Turn turn) {
        return (turn == Turn::White) ? Turn::Black : Turn::White;
    }

    std::vector<bitboard::cordinate>
        bitboard::cordinate_from_bit_representation(bitboard::bit_representation bits) {
        std::vector<bitboard::cordinate> cordinates;

        while (bits != 0x0000'0000) {
            const cordinate cordinate_from_bits {bits};
            cordinates.emplace_back(cordinate_from_bits);
            const std::uint64_t bit_mask {~std::bit_floor(bits)};

            bits = bits & bit_mask;
        }

        return cordinates;
    }

    bitboard::piece bitboard::pieces::from_symbol(char symbol) {
        return *std::find_if(
            pieces::all_pieces.begin(), pieces::all_pieces.end(),
            [symbol](const piece& chess_piece) { return chess_piece.symbol == symbol; });
    }

    bitboard::piece bitboard::pieces::from_index(std::size_t index) {
        return pieces::all_pieces.at(index);
    }

    bitboard::piece bitboard::pieces::from_type_and_turn(PieceType type, Turn turn) {
        return *std::find_if(pieces::all_pieces.begin(), pieces::all_pieces.end(),
                             [type, turn](const piece& chess_piece) {
                                 return chess_piece.piece_type == type && chess_piece.turn == turn;
                             });
    }

    bitboard& bitboard::remove_piece_at_square(const bitboard::bit_representation& bits,
            const piece& piece_removed) {

        _bitboards.at(piece_removed.bitboard_index) &= ~bits;

        return *this;
    }

    bitboard& bitboard::remove_piece_at_square(const bitboard::cordinate& cord,
            const piece& piece_removed) {
        return remove_piece_at_square(cord.to_bit_representation(), piece_removed);
    }

    bitboard& bitboard::remove_piece_at_square(const bitboard::cordinate& cord) {
        const piece piece_removed {piece_at_square(cord)};
        return remove_piece_at_square(cord, piece_removed);
    }

    bitboard& bitboard::remove_piece_at_square(const bitboard::bit_representation& bits) {
        const piece piece_removed {piece_at_square(bits)};
        return remove_piece_at_square(bits, piece_removed);
    }

    bitboard& bitboard::add_piece_at_square(const bitboard::bit_representation& bits,
            const bitboard::piece& piece_added) {

        _bitboards.at(piece_added.bitboard_index) |= bits;

        return *this;
    }

    bitboard& bitboard::add_piece_at_square(const bitboard::cordinate& cord,
            const bitboard::piece& piece_added) {
        return add_piece_at_square(cord.to_bit_representation(), piece_added);
    }

    bitboard::bitboard(const bitboard::chess_grid& grid) {
        for (std::size_t y_cord {0}; y_cord < grid.size(); ++y_cord) {
            for (std::size_t x_cord {0}; x_cord < grid.at(y_cord).size(); ++x_cord) {

                const bitboard::cordinate cord {static_cast<int>(x_cord), static_cast<int>(y_cord)};
                const bitboard::piece chess_piece {grid.at(y_cord).at(x_cord)};

                if (chess_piece != pieces::empty_piece) {
                    add_piece_at_square(cord, chess_piece);
                }
            }
        }
    }

    bitboard::bitboard(const std::string& fen_position) {
        const std::string fen_position_trimmed {fen_position.substr(0, fen_position.find(' '))};

        std::size_t x_cord {0};
        std::size_t y_cord {0};

        for (const char& symbol: fen_position_trimmed) {
            if (symbol == '/') {
                y_cord++;
                x_cord = 0;
            }

            else if (std::isdigit(static_cast<unsigned char>(symbol)) != 0) { // symbol is a digit
                x_cord += std::stoi(std::string {symbol});
            }

            else {
                const bitboard::cordinate cord {static_cast<int>(x_cord), static_cast<int>(y_cord)};
                const bitboard::piece chess_piece {pieces::from_symbol(symbol)};

                add_piece_at_square(cord, chess_piece);
                x_cord++;
            }
        }
    }

    const bitboard::cached_moves_listing_t& bitboard::cached_moves_listing() const {
        return _cached_moves_listing;
    }
} // namespace esochess
