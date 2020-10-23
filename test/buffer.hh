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

#ifndef FIX_TEST_BUFFER_HH
#define FIX_TEST_BUFFER_HH

#include <cstddef>
#include <vector>
#include <string>

namespace fix::test {

class buffer final {
public:
    buffer() noexcept = default;

    std::byte* append(std::size_t size)
    {
        const auto n = data_.size();
        data_.resize(n + size);
        return &data_[0] + n;
    }

    std::byte* append(const std::byte* data, std::size_t size)
    {
        auto it = data_.insert(data_.end(), data, data + size);
        return &(*it);
    }

    void clear()
    {
        data_.clear();
    }

    std::string as_string() const
    {
        return std::string{
            data_.empty() ? "" : reinterpret_cast<const char*>(&data_[0]),
            data_.size()
        };
    }

private:
    std::vector<std::byte> data_;
};

} // namespace fix::test

#endif // FIX_TEST_BUFFER_HH
