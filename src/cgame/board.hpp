#pragma once

#include "prototyped.hpp"
#include "basics.hpp"

struct chess_board {
  std::array<std::array<piece, 8>, 8> board;

  int turn;

  struct {
    bool white_king, black_king, white_queen, black_queen;
  } c_rights;

  piece& at_cord(cordinate);

  bool is_legal();
  std::vector<chess_move> controlled_from(cordinate);
  std::vector<chess_move> controlled_squares();
  std::vector<chess_move> available_moves();
  void make_move(); 
};



