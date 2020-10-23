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

#ifndef FIX_STORE_HEADER_HH
#define FIX_STORE_HEADER_HH

#include <string_view>
#include <vector>
#include <cstddef>
#include <cassert>
#include <cstring>

namespace fix {

class header final {
public:
    header(
        std::string_view proto,
        std::string_view sender,
        std::string_view target
    ) noexcept;

    void append(std::string_view data);

    template <typename T>
    void store(T& buffer, char type) const
    {
        assert(!data_.empty());
        auto p = buffer.append(&data_.front(), data_.size());
        p[begin_off_] = std::byte(type);
    }

    template <typename T>
    void store(T& buffer, std::string_view type) const
    {
        auto off = begin_off_;
        auto dst = buffer.append(data_.size() + type.size() - 1);
        auto src = data_.data();
        std::memcpy(dst, src, off);
        dst += off;
        std::memcpy(dst, type.data(), type.size());
        dst += type.size();
        src += off;
        std::memcpy(
            dst, src + 1,
            static_cast<std::size_t>(&data_.back() - src)
        );
    }

private:
    std::vector<std::byte> data_;
    unsigned int begin_off_{};
};

} // namespace fix

#endif // FIX_STORE_HEADER_HH
