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

#include <fix/store/context.hh>
#include <fix/store/chrono.hh>
#include <fix/checksum.hh>
#include <fix/config.hh>

namespace fix {

static std::vector<std::byte>&
operator<<(std::vector<std::byte>& dst, std::string_view src)
{
    auto p = reinterpret_cast<const std::byte*>(src.data());
    dst.insert(dst.end(), p, p + src.size());
    return dst;
}

context::context(
    std::string_view proto,
    std::string_view sender,
    std::string_view target
) noexcept {
    const auto total_size = (
        proto.size() + sender.size() + target.size() +
        max_body_length_digits() + 21
    );
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
    data_ << "\00149=" << sender << "\00156=" << target
          << "\001";
}

std::byte* context::append(std::size_t size)
{
    const auto offset = data_.size();
    data_.resize(offset + size);
    return &data_.at(offset);
}

std::byte* context::append(const std::byte* data, std::size_t size)
{
    data_.reserve(data_.size() + size);
    const auto offset = data_.size();
    data_.insert(data_.end(), data, data + size);
    return &data_.at(offset);
}

void context::append(std::string_view data)
{
    append(reinterpret_cast<const std::byte*>(data.data()), data.size());
}

void context::store_tail(std::byte* data, std::byte* last_tag) const
{
    auto begin = (data + (begin_off_ - 3));

    assert(last_tag > begin);
    const auto body_length = static_cast<unsigned int>(last_tag - begin);
    begin -= (max_body_length_digits() + 1);

    num2str(begin, body_length, max_body_length_digits());

    last_tag[0] = std::byte{'1'};
    last_tag[1] = std::byte{'0'};
    last_tag[2] = std::byte{'='};
    last_tag[6] = std::byte{'\1'};

    num2str(
        last_tag + 3,
        checksum(data, static_cast<std::size_t>(last_tag - data)),
        3
    );
}

} // namespace fix
