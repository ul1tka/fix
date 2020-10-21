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

#ifndef FIX_LOAD_DECIMAL_HH
#define FIX_LOAD_DECIMAL_HH

#include "../compiler.hh"
#include "../decimal.hh"
#include <cstddef>
#include <cassert>

namespace fix {

inline const std::byte*
load(decimal& dst, const std::byte* data) noexcept
{
    int sign;
    decimal::coefficient_type value{};
    const std::byte* spot{};

    if (FIX_UNLIKELY(*data == std::byte{'-'})) {
        sign = -1;
        ++data;
    } else {
        sign = 1;
    }

    auto c = std::to_integer<unsigned int>(*data);
    do {
        assert(c == '.' || (c >= '0' && c <= '9'));
        if (FIX_LIKELY(c != '.')) {
            value = value * 10 + (c - '0');
        } else {
            spot = data + 1;
        }
        c = std::to_integer<unsigned int>(*++data);
    } while (c != '\1');

    dst = decimal{
        value * sign,
        static_cast<decimal::exponent_type>(spot != nullptr ? (spot - data) : 0)
    };

    return ++data;
}

} // namespace fix

#endif // FIX_LOAD_DECIMAL_HH
