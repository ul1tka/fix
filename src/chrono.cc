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

#include <fix/chrono.hh>

namespace fix {

datetime::datetime(std::time_t seconds, std::uint16_t ms) noexcept
    : ms_{ms}
{
    assert(ms_ < 1000);
    std::tm tm;
    gmtime_r(&seconds, &tm);
    year_ = static_cast<std::uint16_t>(1900 + tm.tm_year);
    month_ = static_cast<std::uint8_t>(tm.tm_mon + 1);
    day_ = static_cast<std::uint8_t>(tm.tm_mday);
    hours_ = static_cast<std::uint8_t>(tm.tm_hour);
    minutes_ = static_cast<std::uint8_t>(tm.tm_min);
    seconds_ = static_cast<std::uint8_t>(tm.tm_sec);
}

} // namespace fix
