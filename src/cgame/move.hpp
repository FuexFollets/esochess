#pragma once

#include "constant_literals.hpp"
#include "prototyped.hpp"
#include "basics.hpp"

struct chess_move {
    int _move_type;

    cordinate _start; // normalized start position

    cordinate _end; // normal end pos

    int _side; // en passant side 

    piece _promotion_piece; // pawn promotion

    int _rook_start;
    int _king_start;
    int _castle_type;

    chess_move() {}

    chess_move(cordinate start, cordinate end) : 
        _start{start}, _end{end}, _move_type{MTYPES.NORMAL} {}

    chess_move(cordinate start, int side) :
        _start{start}, 
        _side{side}, 
        _move_type{MTYPES.EN_PASSANT} {}

    chess_move(cordinate start, piece promotion_piece) :
        _promotion_piece{promotion_piece}, 
        _start{start}, 
        _move_type{MTYPES.PROMOTION} {}

    chess_move(int castle_type, int rook_start=8, int king_start) :
        _castle_type{castle_type}, 
        _rook_start{
            (rook_start == 8) ?
            ((castle_type == CTYPES.KINGSIDE) ? 7 : 0) :
            (rook_start)
        }
};

