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

#ifndef FIX_LOAD_BYTE_HH
#define FIX_LOAD_BYTE_HH

#include <cstddef>
#include <cassert>

namespace fix {

inline const std::byte*
load(std::byte& value, const std::byte* data) noexcept
{
    value = *data;
    assert(data[1] == std::byte{'\1'});
    return data + 2;
}

} // namespace fix

#endif // FIX_LOAD_BYTE_HH
