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

#ifndef FIX_CAPACITOR_HH
#define FIX_CAPACITOR_HH

#include "compiler.hh"
#include <cstddef>
#include <cassert>
#include <vector>

namespace fix {

class capacitor final {
public:
    capacitor() noexcept = default;

    template <typename F>
    void add(const std::byte* FIX_RESTRICT begin,
             const std::byte* FIX_RESTRICT end,
             F&& handler) noexcept;

private:
    int state_{};
    int length_{};
    std::vector<std::byte> data_;
};

template <typename F>
inline void
capacitor::add(const std::byte* FIX_RESTRICT begin,
               const std::byte* FIX_RESTRICT end,
               F&& handler) noexcept
{
    while (begin < end) {
        assert(state_ >= 0);
        if (state_ < 2) {
          find_tag_9:
            assert(length_ == 0);
            assert(state_ < 2);
            assert(begin < end);
            do {
                if (*begin++ == std::byte{'='}) {
                    if (++state_ == 2) {
                        if (FIX_LIKELY(begin < end))
                            goto parse_length;
                        break;
                    }
                }
            } while (FIX_LIKELY(begin < end));
            break;
        } else if (state_ == 3) {
            goto parse_message;
        }
      parse_length:
        assert(state_ == 2);
        assert(begin < end);
        do {
            const auto n = std::to_integer<int>(*begin++);
            if (n == 1) {
                length_ += 7;
                state_ = 3;
                if (FIX_LIKELY(begin < end))
                    goto parse_message;
                break;
            } else {
                assert(n >= '0');
                assert(n <= '9');
                length_ = (length_ * 10) + (n - 48);
            }
        } while (begin < end);
        break;
      parse_message:
        assert(state_ == 3);
        assert(length_ >= 7);
        assert(static_cast<std::size_t>(length_) >= data_.size());
        const auto next =
            begin + (static_cast<std::size_t>(length_) - data_.size());
        if (FIX_LIKELY(next <= end)) {
            const std::byte* msg_begin;
            const std::byte* msg_end;
            if (FIX_UNLIKELY(!data_.empty())) {
                data_.insert(data_.end(), begin, next);
                assert(data_.size() == static_cast<std::size_t>(length_));
                msg_begin = &data_[0];
                msg_end = &data_[0] + length_;
            } else {
                msg_begin = begin;
                msg_end = next;
            }
            handler(msg_begin, msg_end);
            data_.clear();
            length_ = 0;
            state_ = 0;
            begin = next;
            if (begin < end)
                goto find_tag_9;
        } else {
            data_.insert(data_.end(), begin, end);
            assert(static_cast<std::size_t>(length_) > data_.size());
        }
        break;
    }
}

} // namespace fix

#endif // FIX_CAPACITOR_HH
