#ifndef ESOCHESS_BITBOARD_HPP
#define ESOCHESS_BITBOARD_HPP
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

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

            std::size_t bitboard_index;
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
                white_pawn {PieceType::Pawn, Turn::White, 'P', 0},
                white_knight {PieceType::Knight, Turn::White, 'N', 1},
                white_bishop {PieceType::Bishop, Turn::White, 'B', 2},
                white_rook {PieceType::Rook, Turn::White, 'R', 3},
                white_queen {PieceType::Queen, Turn::White, 'Q', 4},
                white_king {PieceType::King, Turn::White, 'K', 5},
                black_pawn {PieceType::Pawn, Turn::Black, 'p', 6},
                black_knight {PieceType::Knight, Turn::Black, 'n', 7},
                black_bishop {PieceType::Bishop, Turn::Black, 'b', 8},
                black_rook {PieceType::Rook, Turn::Black, 'r', 9},
                black_queen {PieceType::Queen, Turn::Black, 'q', 10},
                black_king {PieceType::King, Turn::Black, 'k', 11},
                empty_piece {PieceType::Pawn, Turn::None, ' ', std::string::npos};

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

        struct en_passant_square {
            std::uint8_t column_index;
            Turn captureable_piece_color;

            bool operator==(const en_passant_square& other) const = default;
        };

        struct castle_rights_collection {
            bool white_king_side;
            bool white_queen_side;
            bool black_king_side;
            bool black_queen_side;

            bool operator==(const castle_rights_collection& other) const = default;
        };

        enum class MoveTypes {
            Normal, Capture, EnPassant, Castle, Promotion, PromotionCapture
        };

        struct move_normal_base {
            bit_representation from;
            bit_representation to;
        };

        struct move_capture_base {
            bit_representation from;
            bit_representation to;
        };


        struct move_en_passant_base {
            Turn turn;
        };

        struct move_castle_base {
            enum class CastleType {KingSide, QueenSide};

            Turn turn;
            CastleType castle_type;
        };

        struct move_promotion_base {
            bit_representation from;
            PieceType promotion_type;
        };

        struct move_promotion_capture_base {
            bit_representation from;
            PieceType promotion_type;
        };

        template <MoveTypes _move_type> struct move :
            std::conditional_t<_move_type == MoveTypes::Normal, move_normal_base, void>,
            std::conditional_t<_move_type == MoveTypes::Capture, move_capture_base, void>,
            std::conditional_t<_move_type == MoveTypes::EnPassant, move_en_passant_base, void>,
            std::conditional_t<_move_type == MoveTypes::Castle, move_castle_base, void>,
            std::conditional_t<_move_type == MoveTypes::Promotion, move_promotion_base, void>,
            std::conditional_t<_move_type == MoveTypes::PromotionCapture, move_promotion_capture_base, void> {

            static constexpr MoveTypes move_type {_move_type};
        };

        bitboard& make_move(const move<MoveTypes::Normal>& move);
        bitboard& make_move(const move<MoveTypes::Capture>& move);
        bitboard& make_move(const move<MoveTypes::EnPassant>& move);
        bitboard& make_move(const move<MoveTypes::Castle>& move);
        bitboard& make_move(const move<MoveTypes::Promotion>& move);
        bitboard& make_move(const move<MoveTypes::PromotionCapture>& move);

        bitboard() = default;
        bitboard(const bitboard& other) = default;
        bitboard(bitboard&& other) = default;
        explicit bitboard(const std::string& fen_position);

        bitboard& operator=(const bitboard& other) = default;
        bool operator==(const bitboard& other) const = default;
        bool operator!=(const bitboard& other) const = default;

        [[nodiscard]] std::array<std::array<piece, 8>, 8> to_array() const;
        [[nodiscard]] std::string to_fen();

        [[nodiscard]] std::array<bit_representation, 12>& bitboards() const;
        [[nodiscard]] Turn turn() const;
        [[nodiscard]] en_passant_square en_passant() const;
        [[nodiscard]] castle_rights_collection castle_rights() const;
        [[nodiscard]] int halfmove_clock() const;
        [[nodiscard]] int fullmove_clock() const;

        private:

        std::array<bit_representation, 12> _bitboards;
        Turn _turn;
        en_passant_square _en_passant;
        castle_rights_collection _castle_rights;
        int _halfmove_clock;
        int _fullmove_number;
    };
}

#endif
