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

#ifndef FIX_STORE_NUMBER_HH
#define FIX_STORE_NUMBER_HH

#include "../digits10.hh"
#include <cstddef>
#include <cstring>
#include <cassert>

namespace fix {

inline void
num2str(std::byte* p, std::uint64_t v, unsigned int x) noexcept
{
    assert(x > 0);
    p[x] = std::byte{'\1'};
    for (;;) {
        const auto q = v / 10;
        const auto r = v % 10;
        p[--x] = std::byte(r + '0');
        if (!x)
            break;
        v = q;
    }
}

inline void
num2str(std::byte* p, std::uint64_t v) noexcept
{
    return num2str(p, v, digits10(v));
}

template <typename Buffer>
void store(
    Buffer& buffer,
    const std::byte* tag_data, std::size_t tag_len,
    std::uint64_t value
) {
    const auto x = digits10(value);
    const auto p = buffer.append((tag_len + 1) + x);
    std::memcpy(p, tag_data, tag_len);
    num2str(p + tag_len, value, x);
}

} // namespace fix

#endif // FIX_STORE_NUMBER_HH
