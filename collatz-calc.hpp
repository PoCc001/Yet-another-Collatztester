#ifndef COLLATZ_CALC_HPP
#define COLLATZ_CALC_HPP

/**
 * This file is part of Yet Another Collatztester.
 *
 *  Copyright (C) 2021  Johannes Kloimböck
 *
 *  Yet Another Collatztester is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Yet Another Collatztester is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Yet Another Collatztester.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "carpcontainer.hpp"
#include <intrin.h>

#define trailingZerosCount(x) (_tzcnt_u64((x)))

#define is_even_small(x) (!((x) & 1))
#define is_odd_small(x) ((x) & 1)

#define is_even_big(x) ((x).isEven())
#define is_odd_big(x) ((x).isOdd())

#define odd_small(x) ((x) * 3 + 1)
#define even_small(x) ((x) >> 1)
#define even_multiple_small(x, s) ((x) >> (s))
#define even_all_small(x) (even_multiple_small((x), trailingZerosCount((x))))

#define odd_big(x, t) ((x).mul3p1(t))
#define even_big(x) ((x).rshift(1))
#define even_multiple_big(x, s) ((x).rshift((s)))
#define even_all_big(x) (even_multiple_big((x), (x).trailingZerosCount()))

#endif
