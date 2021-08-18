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

#include "tinybigint.hpp"
#include "collatz-calc.hpp"
#include <iostream>
#include <chrono>

constexpr std::uint64_t SMALL_THRESHOLD = UINT64_MAX / 3 - 1;
constexpr std::size_t WORDS = 16; // multiplying WORDS by 64 equals the number of bits tinybigint will use

inline void test(const std::uint64_t& i, tinybigint& big, std::uint64_t* tmp) {
	std::uint64_t small = i;

	while (true) {
		if (small <= SMALL_THRESHOLD) {
			if (small < i) {
				break;
			}

			small = odd_small(small);
			small = even_all_small(small);
		}
		else {
			if (small != 0xffffffffffffffffULL) {
				big.set(small);
				small = 0xffffffffffffffffULL;
			}

			try {
				odd_big(big, tmp);
				even_all_big(big);
			}
			catch (std::string s) {
				std::cout << "The number " << i << " makes the brute-force algorithm exceed the " <<
					(WORDS << 6) << "bit limit!";
				system("PAUSE");
				exit(0);
			}

			if (big.getLength() == 1) {
				small = big.toUInt64();
			}
		}
	}
}

int main() {
	std::uint64_t i = 11ULL; // don't worry about the integers from 1 to 10!
	std::uint64_t small = i;

	std::uint64_t dvp[WORDS];
	tinybigint big{ i, dvp, WORDS };
	std::uint64_t tmp[WORDS + 1];
	tmp[0] = 0ULL;

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (; i < UINT64_MAX; i += 4ULL) {
		small = i;
		if (i % 1000000000ULL == 1ULL) {
      printf("%g\t\n", (double)(i));
      
      if (i % 5000000000ULL == 1ULL) {
			  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        double tmptime = std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() / 1000.0;
			  double timediff = tmptime - fulltime;
			  fulltime = tmptime;
			  printf("\t%fs.\t%fs.\n", fulltime, timediff);
      }
		}

		if (i % 3ULL == 1ULL) {
			continue;
		}

		test(i, big, tmp);
	}
}
