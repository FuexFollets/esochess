#include <algorithm>
#include <array>
#include <bit>
#include <optional>
#include <string>

#include "headers/bitboard.hpp"
#include "headers/move_generation.hpp"

namespace esochess {
    std::string bitboard::piece::to_string() const {
        return std::string {symbol};
    }

    bitboard::Turn bitboard::color_at_square(const bitboard::bit_representation& bit_mask) const {
        for (const bitboard::piece& chess_piece: pieces::all_pieces) {
            if ((_bitboards.at(chess_piece.bitboard_index) & bit_mask) != 0) {
                return chess_piece.turn;
            }
        }

        return Turn::None;
    }

    bitboard::Turn bitboard::color_at_square(const bitboard::cordinate& cord) const {
        return color_at_square(cord.to_bit_representation());
    }

    bitboard::piece bitboard::piece_at_square(const bitboard::bit_representation& bit_mask) const {
        for (const bitboard::piece& chess_piece: pieces::all_pieces) {
            if ((_bitboards.at(chess_piece.bitboard_index) & bit_mask) != 0) {
                return chess_piece;
            }
        }

        return pieces::empty_piece;
    }

    bitboard::piece bitboard::piece_at_square(const bitboard::cordinate& cord) const {
        return piece_at_square(cord.to_bit_representation());
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

    bitboard::Turn bitboard::turn() const {
        return _turn;
    }

    std::optional<bitboard::en_passant_square> bitboard::en_passant() const {
        return _en_passant;
    }

    bitboard::castle_rights_collection bitboard::castle_rights() const {
        return _castle_rights;
    }

    int bitboard::halfmove_clock() const {
        return _halfmove_clock;
    }

    int bitboard::fullmove_number() const {
        return _fullmove_number;
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

    bool bitboard::in_bounds(const bitboard::cordinate& cord) {
        const int x_cordinate {cord.pos_x()};
        const int y_cordinate {cord.pos_y()};

        return (
            (x_cordinate < 8 && x_cordinate >= 0) &&
            (y_cordinate < 8 && y_cordinate >= 0));
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

    bitboard::cordinate bitboard::en_passant_square::to_cordinate() const {
        return cordinate {
            static_cast<int>(column_index),
            (captureable_piece_color == Turn::White) ? 5 : 2,
        };
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

    bitboard& bitboard::xor_piece(const bit_representation& bits, const piece &piece_modified) {
        _bitboards.at(piece_modified.bitboard_index) ^= bits;

        return *this;
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

    bitboard::moves_listing bitboard::available_moves(bitboard::Turn turn) {
        if (_fullmove_number == _cached_moves_listing.full_move_calculated &&
            ((turn == Turn::White && _cached_moves_listing.white_pieces_moves_complete) ||
             (turn == Turn::Black && _cached_moves_listing.black_pieces_moves_complete))) {
            return turn == Turn::White
                       ? _cached_moves_listing.white_pieces.value_or(moves_listing {})
                       : _cached_moves_listing.black_pieces.value_or(moves_listing {});
        }

        moves_listing moves;

        add_pawn_moves(*this, moves);
        add_king_moves(*this, moves);
        add_rook_bishop_queen_moves(*this, moves);
        add_knight_moves(*this, moves);

        _cached_moves_listing.full_move_calculated = _fullmove_number;

        if (turn == Turn::White) {
            _cached_moves_listing.white_pieces = moves;
            _cached_moves_listing.white_pieces_moves_complete = true;
        }

        else {
            _cached_moves_listing.black_pieces = moves;
            _cached_moves_listing.black_pieces_moves_complete = true;
        }

        return moves;
    }

    const bitboard::cached_moves_listing_t& bitboard::cached_moves_listing() const {
        return _cached_moves_listing;
    }

    std::optional<bitboard::bit_representation>
        bitboard::controlled_squares(bitboard::Turn turn) const {
        if ((turn == Turn::White && _cached_moves_listing.white_pieces_bits_complete) ||
            (turn == Turn::Black && _cached_moves_listing.black_pieces_bits_complete)) {
            return turn == Turn::White
                       ? _cached_moves_listing.white_pieces_bits.value_or(bit_representation {})
                       : _cached_moves_listing.black_pieces_bits.value_or(bit_representation {});
        }

        return std::nullopt;
    }

    bitboard::bit_representtion bitboard::bitboard_bitor_accumulation(Turn turn) const {
        bit_representation accumulation {0};

        if (turn == Turn::White) {
            for (const auto& piece: pieces::white_pieces) {
                accumulation |= _bitboards.at(piece.bitboard_index);
            }
        }

        if (turn == Turn::Black) {
            for (const auto& piece: pieces::black_pieces) {
                accumulation |= _bitboards.at(piece.bitboard_index);
            }
        }

        if (turn == Turn::All) {
            for (const auto& piece: pieces::all_pieces) {
                accumulation |= _bitboards.at(piece.bitboard_index);
            }
        }

        return accumulation;
    }
} // namespace esochess
