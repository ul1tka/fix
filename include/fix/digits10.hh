//
// Copyright (C) 2020 Vlad Lazarenko <vlad@lazarenko.me>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//

#ifndef FIX_DIGITS10_HH
#define FIX_DIGITS10_HH

#include <climits>
#include <cstdint>
#include <cassert>

namespace fix {

namespace detail {

extern const std::uint64_t pow10[20];

} // namespace detail

inline unsigned int
digits10(std::uint64_t value) noexcept
{
    if (value == 0)
        return 1;
    const auto bits = (
        static_cast<int>(sizeof(value) * CHAR_BIT - 1) -
        __builtin_clzll(value)
    );
    const auto length = ((bits * 77) / 256) + 1;
    assert(length >= 0);
    assert(
        static_cast<std::size_t>(length) <
        (sizeof(detail::pow10) / sizeof(detail::pow10[0]))
    );
    return static_cast<unsigned int>(length + (value >= detail::pow10[length]));
}

} // namespace fix

#endif
