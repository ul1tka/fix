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

#include <fix/store/header.hh>
#include <fix/config.hh>

namespace fix {

static std::vector<std::byte>&
operator<<(std::vector<std::byte>& dst, std::string_view src)
{
    auto p = reinterpret_cast<const std::byte*>(src.data());
    dst.insert(dst.end(), p, p + src.size());
    return dst;
}

header::header(
    std::string_view proto,
    std::string_view sender,
    std::string_view target
) noexcept {
    const auto total_size = (proto.size() + sender.size() + target.size() +
                             max_body_length_digits() + 21);
    data_.reserve(total_size);
    data_ << "8=";
    for (const auto c : proto)
        data_.emplace_back(std::byte(c));
    data_ << "\0019=";
    for (unsigned int i = 0; i < max_body_length_digits(); ++i)
        data_.emplace_back(std::byte{' '});
    data_ << "\00135=";
    begin_off_ = static_cast<unsigned int>(data_.size());
    data_.emplace_back(std::byte{' '});
    data_ << "\00149=" << sender << "\00156=" << target << "\00134=";
}

std::byte* header::append(std::size_t size)
{
    data_.reserve(data_.size() + size);
    data_.resize(data_.size() - 3);
    const auto offset = data_.size();
    data_.resize(data_.size() + size);
    data_ << "34=";
    return &data_.at(offset);
}

std::byte* header::append(const std::byte* data, std::size_t size)
{
    data_.reserve(data_.size() + size);
    data_.resize(data_.size() - 3);
    const auto offset = data_.size();
    data_.insert(data_.end(), data, data + size);
    data_ << "34=";
    return &data_.at(offset);
}

void header::append(std::string_view data)
{
    append(reinterpret_cast<const std::byte*>(data.data()), data.size());
}

} // namespace fix
