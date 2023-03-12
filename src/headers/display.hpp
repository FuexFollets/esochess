#ifndef ESOCHESS_DISPLAY_HPP
#define ESOCHESS_DISPLAY_HPP
#include <cstdint>
#pragma once

#include "bitboard.hpp"

namespace esochess {
    void fancy_display(std::uint64_t square);
    void fancy_display(const bitboard& displayed_bitboard);
}

#endif
