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

#ifndef FIX_STORE_CONTEXT_HH
#define FIX_STORE_CONTEXT_HH

#include "number.hh"
#include "sending_time.hh"
#include <string_view>
#include <array>
#include <vector>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <cstring>

namespace fix {

class context final {
public:
    context() noexcept = default;

    context(
        std::string_view proto,
        std::string_view sender,
        std::string_view target
    ) noexcept;

    void append(std::string_view data);

    std::byte* append(std::size_t size);

    std::byte* append(const std::byte* data, std::size_t size);

    template <typename T>
    void store_head(T& buffer, char type, const sending_time& timestamp,
                    std::uint64_t sequence) const
    {
        assert(!data_.empty());
        const auto x = digits10(sequence);
        const auto n = data_.size();
        auto p = buffer.append(&data_.front(), n + x + 1 + timestamp.size());
        p[begin_off_] = std::byte(type);
        std::memcpy(p + n, timestamp.data(), timestamp.size());
        num2str(p + n + timestamp.size(), sequence, x);
    }

    template <typename T>
    void store_head(T& buffer, std::string_view type,
                    const sending_time& timestamp,
                    std::uint64_t sequence) const
    {
        const auto dig = digits10(sequence);
        store_head(
            buffer.append(data_.size() + timestamp.size() + type.size() + dig),
            dig, type, timestamp, sequence
        );
    }

    template <typename T>
    void store_tail(T& buffer) const
    {
        auto last_tag = buffer.append(7);
        store_tail(static_cast<std::byte*>(buffer.data()),
                   static_cast<std::byte*>(last_tag));
    }

private:
    void store_tail(std::byte* data, std::byte* last_tag) const;

    void store_head(std::byte* dst, unsigned int dig,
                    std::string_view type, const sending_time& timestamp,
                    std::uint64_t sequence) const;

    /// @todo Allocate all at once.
    std::vector<std::byte> data_;
    unsigned int begin_off_{};
};

} // namespace fix

#endif // FIX_STORE_CONTEXT_HH
