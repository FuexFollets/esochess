#ifndef ESOCHESS_BITBOARD_HPP
#define ESOCHESS_BITBOARD_HPP
#pragma once

#include <array>
#include <cstdint>
#include <string>

namespace esochess {
    struct bitboard {
        using bit_representation = std::uint64_t;

        enum class Turn {White, Black, None};
        enum class PieceType {Pawn, Knight, Bishop, Rook, Queen, King};

        static constexpr const char* const starting_position_fen {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};

        struct piece {
            [[nodiscard]] std::string to_string() const;

            bool operator!=(const piece& other) const = default;
            bool operator==(const piece& other) const = default;

            PieceType piece_type;
            Turn turn;
            char symbol;
        };

        struct cordinate {
            cordinate() = default;
            cordinate(int pos_x, int pos_y);
            explicit cordinate(bit_representation bit_mask);

            bool operator==(const cordinate& other) const = default;
            bool operator!=(const cordinate& other) const = default;

            [[nodiscard]] int pos_x() const;
            [[nodiscard]] int pos_y() const;

            [[nodiscard]] bit_representation to_bit_representation() const;
            [[nodiscard]] std::string to_string() const;

            private:

            int _x;
            int _y;
        };

        struct pieces {
            static constexpr piece
                white_pawn {PieceType::Pawn, Turn::White, 'P'},
                white_knight {PieceType::Knight, Turn::White, 'N'},
                white_bishop {PieceType::Bishop, Turn::White, 'B'},
                white_rook {PieceType::Rook, Turn::White, 'R'},
                white_queen {PieceType::Queen, Turn::White, 'Q'},
                white_king {PieceType::King, Turn::White, 'K'},
                black_pawn {PieceType::Pawn, Turn::Black, 'p'},
                black_knight {PieceType::Knight, Turn::Black, 'n'},
                black_bishop {PieceType::Bishop, Turn::Black, 'b'},
                black_rook {PieceType::Rook, Turn::Black, 'r'},
                black_queen {PieceType::Queen, Turn::Black, 'q'},
                black_king {PieceType::King, Turn::Black, 'k'},
                empty_piece {PieceType::Pawn, Turn::None, ' '};

            static constexpr std::array<piece, 12> all_pieces {
                white_pawn, white_knight, white_bishop, white_rook, white_queen, white_king,
                black_pawn, black_knight, black_bishop, black_rook, black_queen, black_king
            };

            static constexpr std::array<piece, 6> white_pieces {
                white_pawn, white_knight, white_bishop, white_rook, white_queen, white_king
            };

            static constexpr std::array<piece, 6> black_pieces {
                black_pawn, black_knight, black_bishop, black_rook, black_queen, black_king
            };
        };

        struct castle_rights {
            bool white_king_side;
            bool white_queen_side;
            bool black_king_side;
            bool black_queen_side;
        };

        bitboard() = default;
        bitboard(const bitboard& other) = default;
        bitboard(bitboard&& other) = default;
        explicit bitboard(const std::string& fen_position);
    };
}

#endif
