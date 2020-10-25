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

#ifndef FIX_CHECKSUM_HH
#define FIX_CHECKSUM_HH

#include <cstddef>

namespace fix {

/// @todo size should be unsigned int
constexpr unsigned int
checksum(const std::byte* data, std::size_t size) noexcept
{
    unsigned int s{};
    for (std::size_t i = 0; i < size; ++i)
        s += std::to_integer<unsigned int>(data[i]);
    return s % 256;
}

} // namespace fix

#endif // FIX_CHECKSUM_HH
