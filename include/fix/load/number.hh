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

#ifndef FIX_LOAD_NUMBER_HH
#define FIX_LOAD_NUMBER_HH

#include <tuple>
#include <type_traits>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cassert>

namespace fix {

template <char S = '\1'>
inline std::tuple<std::uint64_t, const std::byte*>
load_u64(const std::byte* data) noexcept
{
    auto q = reinterpret_cast<const unsigned char*>(data);
    assert(*q >= '0');
    assert(*q <= '9');
    auto v = static_cast<std::uint64_t>(*q++ - '0');
    for (auto c = *q++; c != S; c = *q++) {
        assert(c >= '0');
        assert(c <= '9');
        v = (v * 10) + (c - '0');
    }
    return { v, reinterpret_cast<const std::byte*>(q) };
}

inline const std::byte*
load(std::uint64_t& value, const std::byte* data) noexcept
{
    std::tie(value, data) = load_u64(data);
    return data;
}

inline const std::byte*
load(std::int64_t& value, const std::byte* data) noexcept
{
    const auto is_negative = (*data == std::byte{'-'});
    std::uint64_t u;
    data = load(u, data + (is_negative ? 1 : 0));
    value = static_cast<std::int64_t>(u) * (is_negative ? -1 : 1);
    return data;
}

inline const std::byte*
load(std::uint32_t& value, const std::byte* data) noexcept
{
    std::uint64_t v;
    data = load(v, data);
    value = static_cast<std::uint32_t>(v);
    return data;
}

inline const std::byte*
load(std::int32_t& value, const std::byte* data) noexcept
{
    std::int64_t v;
    data = load(v, data);
    value = static_cast<std::int32_t>(v);
    return data;
}

inline const std::byte*
load(std::uint16_t& value, const std::byte* data) noexcept
{
    std::uint64_t v;
    data = load(v, data);
    value = static_cast<std::uint16_t>(v);
    return data;
}

inline const std::byte*
load(std::int16_t& value, const std::byte* data) noexcept
{
    std::int64_t v;
    data = load(v, data);
    value = static_cast<std::int16_t>(v);
    return data;
}

} // namespace fix

#endif // FIX_LOAD_NUMBER_HH
