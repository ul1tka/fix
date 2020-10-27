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

#ifndef FIX_STORE_SENDING_TIME_HH
#define FIX_STORE_SENDING_TIME_HH

#include <array>
#include <tuple>

namespace fix {

class datetime;

class sending_time final {
    std::array<std::byte, 28> data_ = {
        std::byte{'5'}, std::byte{'2'}, std::byte{'='}, std::byte{},
        std::byte{}, std::byte{}, std::byte{}, std::byte{}, std::byte{},
        std::byte{}, std::byte{}, std::byte{}, std::byte{}, std::byte{},
        std::byte{}, std::byte{}, std::byte{}, std::byte{}, std::byte{},
        std::byte{}, std::byte{}, std::byte{}, std::byte{}, std::byte{},
        std::byte{1}, std::byte{'3'}, std::byte{'4'}, std::byte{'='}
    };

public:
    constexpr sending_time() noexcept = default;

    void set(const datetime& value) noexcept;

    decltype(auto) data() const noexcept
    {
        return data_.data();
    }

    constexpr decltype(auto) size() const noexcept
    {
        return data_.size();
    }
};

} // namespace fix

#endif // FIX_STORE_SENDING_TIME_HH
