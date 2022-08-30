#include <bits/stdc++.h>

struct cordinate;
struct piece;
struct chess_move;
struct chess_board;

cordinate operator+(cordinate);

void chess_board::get_turn();
void chess_board::at_cord(cordinate);
std::unordered_map<chess_move> chess_board::available_moves();
std::unordered_map<chess_move> chess_board::controlled_squares();
std::unordered_map<chess_move> chess_board::controlled_from(cordinate);

int opposite_color(int);
