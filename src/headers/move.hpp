#ifndef ESOCHESS_MOVE_HPP
#define ESOCHESS_MOVE_HPP
#pragma once

#include <cstdint>

#include "bitboard.hpp"

namespace esochess {
    struct move {
        bitboard::piece piece_moved;
        std::uint64_t bit_mask;
    };
}

#endif
