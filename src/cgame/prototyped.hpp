#pragma once

#include <bits/stdc++.h>

struct cordinate;
struct piece;
struct chess_move;
struct chess_board;

/*
cordinate cordinate::operator+(cordinate);
cordinate cordinate::operator-(cordinate);

chess_move::chess_move(); // Defualt initializer
chess_move::chess_move(cordinate, cordinate); // Normal move
chess_move::chess_move(cordinate, int); // En passant
chess_move::chess_move(cordinate, piece); // Promotion
chess_move::chess_move(int king_start, int rook_start, int castle_type);

cordinate operator+(cordinate);

void chess_board::get_turn();
void chess_board::at_cord(cordinate);
std::unordered_map<chess_move> chess_board::available_moves();
std::unordered_map<chess_move> chess_board::controlled_squares();
std::unordered_map<chess_move> chess_board::controlled_from(cordinate);

int opposite_color(int);
*/