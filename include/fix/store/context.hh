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
#include <string_view>
#include <vector>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <cstring>

namespace fix {

class datetime;

class context final {
public:
    context() noexcept = default;

    context(
        std::string_view proto,
        std::string_view sender,
        std::string_view target
    ) noexcept;

    void set_time(const datetime& value) noexcept;

    void append(std::string_view data);

    std::byte* append(std::size_t size);

    std::byte* append(const std::byte* data, std::size_t size);

    template <typename T>
    void store(T& buffer, char type, std::uint64_t sequence) const
    {
        assert(!data_.empty());
        const auto x = digits10(sequence);
        const auto n = data_.size();
        auto p = buffer.append(&data_.front(), n + x + 1);
        p[begin_off_] = std::byte(type);
        num2str(p + n, sequence, x);
    }

    template <typename T>
    void store(T& buffer, std::string_view type, std::uint64_t sequence) const
    {
        auto dig = digits10(sequence);
        auto off = begin_off_;
        auto dst = buffer.append(data_.size() + type.size() + dig);
        auto src = data_.data();
        std::memcpy(dst, src, off);
        dst += off;
        std::memcpy(dst, type.data(), type.size());
        dst += type.size();
        src += off;
        auto n = static_cast<std::size_t>(&data_.back() - src);
        std::memcpy(dst, src + 1, n);
        dst += n;
        num2str(dst, sequence, dig);
    }

    void store_tail(std::byte* data, std::byte* last_tag) const;

    template <typename T>
    void store_tail(T& buffer) const
    {
        auto last_tag = buffer.append(7);
        store_tail(
            static_cast<std::byte*>(buffer.data()),
            static_cast<std::byte*>(last_tag)
        );
    }

private:
    /// @todo Allocate all at once.
    std::vector<std::byte> data_;
    unsigned int begin_off_{};
};

} // namespace fix

#define FIX_STORE_BEGIN(Buffer, Context, Type, Sequence)    \
    do {                                                    \
        (Context).store((Buffer), (Type), (Sequence));      \
    } while (false)

#define FIX_STORE_END(Buffer, Context)          \
    do {                                        \
        (Context).store_tail((Buffer));         \
    } while (false)

#endif // FIX_STORE_CONTEXT_HH
