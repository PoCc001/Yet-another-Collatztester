#ifndef TINYBIGINT_HPP
#define TINYBIGINT_HPP

/**
 * This file is part of Yet Another Collatztester.
 *
 *  Copyright (C) 2021 - 2022  Johannes Kloimböck
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

#include <string>
#include <intrin.h>

// use [[likely]] and [[unlikely]] to optimize branch prediction, if a C++20 compiler is used
#if defined(_MSVC_LANG) && _MSVC_LANG > 201703L
#define CPP20LIKELY [[likely]]
#define CPP20UNLIKELY [[unlikely]]
#else
#define CPP20LIKELY
#define CPP20UNLIKELY
#endif

#define EXCEPTION_TEXT "Exceeds size of this tinybigint!"

class tinybigint {
	std::uint64_t* dvp;
	std::size_t size;
	std::size_t length;

public:
	tinybigint() = delete;
	tinybigint(unsigned long long v, std::uint64_t* mem, std::size_t s = 16) {
		if (s < 1) CPP20UNLIKELY {
			throw std::string("The size must be at least 1!");
		}

		dvp = mem;
		dvp[0] = v;
		size = s;
		length = 1;
	}

	tinybigint(const tinybigint& tbi) = default;
	tinybigint(tinybigint&& tmp) noexcept = default;
	~tinybigint() {
		dvp = nullptr;
		size = 0;
		length = 0;
	}

	void set(std::uint64_t v) noexcept {
		dvp[0] = v;
		length = 1;
	}

	void rshift(std::size_t s) noexcept {
		std::size_t words = s >> 6;
		std::size_t index = s & 0x3f;

		std::size_t newLength = length - words;

		bool subtract = (dvp[length - 1] >> index) == 0ULL;

		if (subtract) {
			--newLength;
		}

		for (std::size_t i = 0; i < newLength - 1; ++i) {
			dvp[i] = (dvp[i + words] >> index) | (dvp[i + words + 1] << (64 - index));
		}

		dvp[newLength - 1] = dvp[newLength + words - 1] >> index;

		if (subtract) {
			dvp[newLength - 1] |= dvp[newLength + words] << (64 - index);
		}

		length = newLength;
	}

	std::size_t trailingZerosCount() const noexcept {
		std::size_t cnt = 0;

		while (dvp[cnt] == 0) {
			++cnt;
		}

		std::uint64_t v = dvp[cnt];

		cnt <<= 6;

		std::size_t i = _tzcnt_u64(v);
		cnt += i;

		return cnt;
	}

	void add(std::uint64_t a) {
		std::uint8_t carry = 0;
		carry = _addcarry_u64(carry, dvp[0], a, dvp);
		for (std::size_t i = 0; i < length; ++i) {
			if (!carry) CPP20LIKELY {
				return;
			}
			carry = _addcarry_u64(carry, dvp[i], 0, dvp + i);
		}
	}

	void mul3p1(std::uint64_t* tmp) {
		std::uint8_t carry = 1;
		for (std::size_t i = 0; i < length; ++i) {
			dvp[i] = _umul128(dvp[i], 3ULL, tmp + i + 1);
			carry = _addcarry_u64(carry, dvp[i], tmp[i], dvp + i);
		}

		if (length < size && (carry != 0 || tmp[length] != 0)) {
			dvp[length] = tmp[length] + (std::uint64_t)(carry);
			++length;
		} else if ((carry != 0 || tmp[length] != 0) && length == size) CPP20UNLIKELY {
			throw std::string(EXCEPTION_TEXT);
		}
	}

	std::uint64_t toUInt64() const noexcept {
		return dvp[0];
	}

	bool isEven() const noexcept {
		return !(dvp[0] & 1ULL);
	}

	bool isOdd() const noexcept {
		return dvp[0] & 1ULL;
	}

	bool isOne() const noexcept {
		return length == 1 && dvp[0] == 1ULL;
	}

	std::size_t getLength() const noexcept {
		return length;
	}
};

#endif
