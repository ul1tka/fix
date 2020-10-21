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

#ifndef FIX_LOAD_CHAR_HH
#define FIX_LOAD_CHAR_HH

#include <type_traits>
#include <cstddef>
#include <cassert>

namespace fix {

template <
    typename T,
    std::enable_if_t<
        std::is_same_v<T, char> ||
        std::is_same_v<T, unsigned char>,
        int
    > = 0
>
inline const std::byte*
load(T& value, const std::byte* data) noexcept
{
    value = std::to_integer<T>(*data);
    assert(value >= ' ');
    assert(value <= '~');
    assert(data[1] == std::byte{'\1'});
    return data + 2;
}

} // namespace fix

#endif // FIX_LOAD_CHAR_HH
