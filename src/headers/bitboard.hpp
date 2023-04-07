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
        enum class Direction { // Ordinal cordinate where `North` tends towards the 8th rank
                               // `South` tends towards the 1st rank
                               // `East` tends towards the 'h' file
                               // `West` tends towards the 'a' file
            North, NorthEast, East, SouthEast,
            South, SouthWest, West, NorthWest};
        enum class CastleType {KingSide, QueenSide};

        static constexpr int white_pawn_starting_rank {1};
        static constexpr int black_pawn_starting_rank {6};

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

            [[nodiscard]] cordinate north(int steps) const;
            [[nodiscard]] cordinate northwest(int steps) const;
            [[nodiscard]] cordinate west(int steps) const;
            [[nodiscard]] cordinate southwest(int steps) const;
            [[nodiscard]] cordinate south(int steps) const;
            [[nodiscard]] cordinate southeast(int steps) const;
            [[nodiscard]] cordinate east(int steps) const;
            [[nodiscard]] cordinate northeast(int steps) const;

            [[nodiscard]] bit_representation to_bit_representation() const;
            [[nodiscard]] std::string to_string() const;
            [[nodiscard]] std::string to_fancy_string() const;

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

            static constexpr std::array<piece, 4> white_pawn_promotion_pieces {
                white_knight, white_bishop, white_rook, white_queen
            };

            static constexpr std::array<piece, 4> black_pawn_promotion_pieces {
                black_knight, black_bishop, black_rook, black_queen
            };

            static piece from_symbol(char symbol);
            static piece from_index(std::size_t index);
            static piece from_type_and_turn(PieceType type, Turn turn);
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

        struct move_normal {
            move_normal() = default;
            move_normal(bit_representation bit_from, bit_representation bit_to);

            static constexpr std::size_t variant_index {0};

            bit_representation from;
            bit_representation to;
        };

        struct move_en_passant {
            move_en_passant() = default;
            move_en_passant(bit_representation bit_from, bit_representation bit_to);

            static constexpr std::size_t variant_index {1};

            Turn turn;
        };

        struct move_castle {
            move_castle() = default;
            move_castle(Turn turn, CastleType castle_type);

            static constexpr std::size_t variant_index {2};

            Turn turn;
            CastleType castle_type;
        };

        struct move_promotion {
            move_promotion() = default;
            move_promotion(bit_representation bit_from, PieceType promotion_type, Direction promotion_direction);

            static constexpr std::size_t variant_index {3};

            Direction promotion_direction;

            bit_representation from;
            PieceType promotion_type;
        };

        using move = std::variant<
            move_normal,
            move_en_passant,
            move_castle,
            move_promotion>;

        bitboard& make_move(const move_normal& move);
        bitboard& make_move(const move_en_passant& move);
        bitboard& make_move(const move_castle& move);
        bitboard& make_move(const move_promotion& move);

        bitboard() = default;
        bitboard(const bitboard& other) = default;
        bitboard(bitboard&& other) = default;
        explicit bitboard(const std::string& fen_position);

        bitboard& operator=(const bitboard& other) = default;
        bool operator==(const bitboard& other) const = default;
        bool operator!=(const bitboard& other) const = default;

        [[nodiscard]] Turn color_at_square(const bit_representation& bit_mask) const;
        [[nodiscard]] Turn color_at_square(const cordinate& cord) const;

        using chess_grid = std::array<std::array<piece, 8>, 8>;
        [[nodiscard]] chess_grid to_grid() const;
        [[nodiscard]] std::string to_fen() const;
        [[nodiscard]] std::string to_fancy_string() const;

        [[nodiscard]] std::array<bit_representation, 12> bitboards() const;
        [[nodiscard]] Turn turn() const;
        [[nodiscard]] en_passant_square en_passant() const;
        [[nodiscard]] castle_rights_collection castle_rights() const;
        [[nodiscard]] int halfmove_clock() const;
        [[nodiscard]] int fullmove_clock() const;

        struct moves_listing {
            std::vector<move_normal> normal_moves;
            std::vector<move_castle> castle_moves;
            std::vector<move_en_passant> en_passant_moves;
            std::vector<move_promotion> promotion_moves;
        };

        [[nodiscard]] moves_listing available_moves(Turn turn) const;
        [[nodiscard]] moves_listing available_moves() const;
        [[nodiscard]] bit_representation controlled_squares(Turn checked_turn) const;

        static Turn opposite_turn(Turn turn);
        static std::vector<cordinate> cordinate_from_bit_representation(bit_representation bits);

        private:

        std::array<bit_representation, 12> _bitboards {};
        Turn _turn {};
        std::optional<en_passant_square> _en_passant {};
        castle_rights_collection _castle_rights {};
        int _halfmove_clock {};
        int _fullmove_number {};

        struct cached_moves_listing_t { // Avoid recalculating the moves listing
            int full_move_calculated; // The fullmove in which these move listing were calculated

            std::optional<moves_listing> white_pieces;
            std::optional<moves_listing> black_pieces;

            std::optional<bit_representation> white_controlled_squares;
            std::optional<bit_representation> black_controlled_squares;

            bool operator==(const cached_moves_listing_t&) const { return true; }
            bool operator!=(const cached_moves_listing_t&) const { return true; }
        } _cached_moves_listing;
    };
}

#endif
