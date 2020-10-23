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

void header::append(std::string_view data)
{
    data_.reserve(data_.size() + data.size());
    data_.resize(data_.size() - 3);
    auto p = reinterpret_cast<const std::byte*>(data.data());
    data_.insert(data_.end(), p, p + data.size());
    data_ << "34=";
}

} // namespace fix
