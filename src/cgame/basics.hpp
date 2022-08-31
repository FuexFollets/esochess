#pragma once

#include "prototyped.hpp"

struct cordinate {
  int _x, _y;

  cordinate() {}

  cordinate(int x, int y) : _x{x}, _y{y} {}

  cordinate operator+ (cordinate cord) {
    return cordinate(cord._x + _x, cord._y + _y);
  }

  cordinate operator- (cordinate cord) {
    return cordinate(cord._x + _x, cord._y + _y);
  }
};


struct piece {
  int _color{3}, _type;

  piece() {}

  piece(int color, int type) : _color{color}, _type{type} {}
};


int opposite_color(int color) {
  return color ^ 1;
}




