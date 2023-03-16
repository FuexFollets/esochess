#include <cstdint>
#include <vector>

#include "headers/bitboard.hpp"
#include "headers/movements.hpp"

namespace esochess {
    bitboard::Turn bitboard::piece_color_at_square(std::uint64_t bit_mask) const {
        for (const piece& white_piece : pieces::all_white_pieces) {
            if (static_cast<bool>(_pieces_bit_board[white_piece.index] & bit_mask)) {
                return Turn::White;
            }
        }

        for (const piece& black_piece : pieces::all_black_pieces) {
            if (static_cast<bool>(_pieces_bit_board[black_piece.index] & bit_mask)) {
                return Turn::Black;
            }
        }

        return Turn::None;
    }

    bitboard::Turn bitboard::piece_color_at_square(const cordinate& cordinate) const {
        return piece_color_at_square(get_bits_from_cordinate(cordinate));
    }

    std::vector<bitboard::move> bitboard::all_maybe_legal_moves() const { // Incomplete
        std::vector<bitboard::move> maybe_legal_moves;

        for (const piece& chess_piece : (_turn == Turn::White ? pieces::all_white_pieces : pieces::all_black_pieces)) {
            for (const cordinate& starting_square : get_cordinates_from_bits(_pieces_bit_board[chess_piece.index])) {
                if (chess_piece == pieces::white_pawn) {
                    for (const cordinate& ending_square : movements_from_square_as_pawn(Turn::White, starting_square.to_bits())) {
                        if ( // Pawn moving one square forward
                                ending_square.x == starting_square.x &&
                                ending_square.y == starting_square.y + 1 &&
                                piece_color_at_square(cordinate {ending_square.x, ending_square.y}) == Turn::None
                           ) {
                            maybe_legal_moves.emplace_back(chess_piece,
                                    get_bits_from_cordinate(starting_square),
                                    get_bits_from_cordinate(ending_square));
                        }

                        if ( // Pawn moving two squares forward
                                ending_square.x == starting_square.x &&
                                ending_square.y == starting_square.y + 2 &&
                                piece_color_at_square(cordinate {ending_square.x, ending_square.y}) == Turn::None &&
                                piece_color_at_square(cordinate {ending_square.x, ending_square.y - 1}) == Turn::None
                           ) {
                            maybe_legal_moves.emplace_back(chess_piece,
                                    get_bits_from_cordinate(starting_square),
                                    get_bits_from_cordinate(ending_square));
                        }

                        if ( // Pawn taking a piece
                                (ending_square.x == starting_square.x + 1 || ending_square.x == starting_square.x - 1) &&
                                ending_square.y == starting_square.y + 1 &&
                                piece_color_at_square(cordinate {ending_square.x, ending_square.y}) == Turn::Black
                           ) {
                            maybe_legal_moves.emplace_back(chess_piece,
                                    get_bits_from_cordinate(starting_square),
                                    get_bits_from_cordinate(ending_square));
                        }

                        if ( // Pawn taking a piece en passant
                                _en_passant_square.has_value() &&
                                (ending_square.x == starting_square.x + 1 || ending_square.x == starting_square.x - 1) &&
                                ending_square.y == starting_square.y + 1 &&
                                piece_color_at_square(cordinate {ending_square.x, ending_square.y - 1}) == Turn::Black &&
                                get_cordinates_from_bits(_en_passant_square.value())[0] == cordinate {ending_square.x, ending_square.y - 1}
                           ) {
                            maybe_legal_moves.emplace_back(chess_piece,
                                    get_bits_from_cordinate(starting_square),
                                    get_bits_from_cordinate(ending_square));
                        }
                    }
                } // End of if statment for white pawn

                else if (chess_piece == pieces::black_pawn) {
                    for (const cordinate& ending_square : movements_from_square_as_pawn(Turn::Black, starting_square.to_bits())) {
                        if ( // Pawn moving one square forward
                                ending_square.x == starting_square.x &&
                                ending_square.y == starting_square.y - 1 &&
                                piece_color_at_square(cordinate {ending_square.x, ending_square.y}) == Turn::None
                           ) {
                            maybe_legal_moves.emplace_back(chess_piece,
                                    get_bits_from_cordinate(starting_square),
                                    get_bits_from_cordinate(ending_square));
                        }

                        if ( // Pawn moving two squares forward
                                ending_square.x == starting_square.x &&
                                ending_square.y == starting_square.y - 2 &&
                                piece_color_at_square(cordinate {ending_square.x, ending_square.y}) == Turn::None &&
                                piece_color_at_square(cordinate {ending_square.x, ending_square.y + 1}) == Turn::None
                           ) {
                            maybe_legal_moves.emplace_back(chess_piece,
                                    get_bits_from_cordinate(starting_square),
                                    get_bits_from_cordinate(ending_square));
                        }

                        if ( // Pawn taking a piece
                                (ending_square.x == starting_square.x + 1 || ending_square.x == starting_square.x - 1) &&
                                ending_square.y == starting_square.y - 1 &&
                                piece_color_at_square(cordinate {ending_square.x, ending_square.y}) == Turn::White
                           ) {
                            maybe_legal_moves.emplace_back(chess_piece,
                                    get_bits_from_cordinate(starting_square),
                                    get_bits_from_cordinate(ending_square));
                        }

                        if ( // Pawn taking a piece en passant
                                _en_passant_square.has_value() &&
                                (ending_square.x == starting_square.x + 1 || ending_square.x == starting_square.x - 1) &&
                                ending_square.y == starting_square.y - 1 &&
                                piece_color_at_square(cordinate {ending_square.x, ending_square.y + 1}) == Turn::White &&
                                get_cordinates_from_bits(_en_passant_square.value())[0] == cordinate {ending_square.x, ending_square.y + 1}
                           ) {
                            maybe_legal_moves.emplace_back(chess_piece,
                                    get_bits_from_cordinate(starting_square),
                                    get_bits_from_cordinate(ending_square));
                        }
                    }
                } // End of if statement for black pawn

                else if (chess_piece == pieces::white_knight || chess_piece == pieces::black_knight) {
                    for (const cordinate& ending_square : movements_from_square_as_knight(starting_square.to_bits())) {
                        if (piece_color_at_square(cordinate {ending_square.x, ending_square.y}) != chess_piece.color) {
                            maybe_legal_moves.emplace_back(chess_piece,
                                    get_bits_from_cordinate(starting_square),
                                    get_bits_from_cordinate(ending_square));
                        }
                    }
                } // End of if statement for knight

                else { // Piece is either bishop, rook, or queen
                    const std::vector<std::vector<cordinate>> directions {
                        (chess_piece == pieces::white_bishop || chess_piece == pieces::black_bishop) ?
                            movements_from_square_as_bishop(starting_square.to_bits()) :
                        (chess_piece == pieces::white_rook || chess_piece == pieces::black_rook) ?
                            movements_from_square_as_rook(starting_square.to_bits()) :
                            movements_from_square_as_queen(starting_square.to_bits())
                    };

                    for (const std::vector<cordinate>& direction : directions) {
                        for (const cordinate& ending_square : direction) {
                            if (piece_color_at_square(ending_square) == chess_piece.color) {
                                break;
                            }

                            maybe_legal_moves.emplace_back(chess_piece,
                                    get_bits_from_cordinate(starting_square),
                                    get_bits_from_cordinate(ending_square));

                            if (piece_color_at_square(ending_square) == opposite_turn(chess_piece.color)) {
                                break;
                            }
                        }
                    }
                }
            }
        }

        return maybe_legal_moves;
    }
}
