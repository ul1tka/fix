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

#ifndef FIX_LOAD_RAW_HH
#define FIX_LOAD_RAW_HH

#include "number.hh"
#include <string_view>

namespace fix {

inline const std::byte*
load_raw(const std::byte* data, std::size_t offset) noexcept
{
    unsigned int size{};
    const auto p = load(size, data);
    return p + (offset + size + 1);
}

inline const std::byte*
load_raw(std::string_view& field, const std::byte* data,
         std::size_t offset) noexcept
{
    unsigned int size{};
    const auto p = load(size, data) + offset;
    field = std::string_view{reinterpret_cast<const char*>(p), size};
    return p + (size + 1);
}

} // namespace fix

#endif // FIX_LOAD_RAW_HH
