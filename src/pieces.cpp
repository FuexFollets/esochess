#include <algorithm>

#include "headers/bitboard.hpp"

namespace esochess {
    bitboard::piece bitboard::pieces::from_symbol(char symbol) {
        switch (symbol) {
            case 'P': return bitboard::pieces::white_pawn;
            case 'N': return bitboard::pieces::white_knight;
            case 'B': return bitboard::pieces::white_bishop;
            case 'R': return bitboard::pieces::white_rook;
            case 'Q': return bitboard::pieces::white_queen;
            case 'K': return bitboard::pieces::white_king;
            case 'p': return bitboard::pieces::black_pawn;
            case 'n': return bitboard::pieces::black_knight;
            case 'b': return bitboard::pieces::black_bishop;
            case 'r': return bitboard::pieces::black_rook;
            case 'q': return bitboard::pieces::black_queen;
            case 'k': return bitboard::pieces::black_king;
            default: return bitboard::pieces::empty_piece;
        }

        return empty_piece;
    }

    bitboard::piece bitboard::pieces::from_index(std::size_t index) {
        return bitboard::pieces::all_pieces.at(index);
    }

    bitboard::piece bitboard::pieces::from_type_and_turn(PieceType type, Turn turn) {
        return *std::find_if(
            bitboard::pieces::all_pieces.begin(),
            bitboard::pieces::all_pieces.end(),
            [type, turn](bitboard::piece piece) {
                return piece.piece_type == type && piece.turn == turn;
            }
        );
    }


}
