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

#ifndef FIX_STORE_CHRONO_HH
#define FIX_STORE_CHRONO_HH

#include "../chrono.hh"
#include <cstring>
#include <cstddef>
#include <array>

namespace fix {

inline void store(std::byte* q, const datetime& v) noexcept
{
    q[0] = static_cast<std::byte>(((v.year() / 1000) % 10) + '0');
    q[1] = static_cast<std::byte>(((v.year() / 100) % 10) + '0');
    q[2] = static_cast<std::byte>(((v.year() / 10) % 10) + '0');
    q[3] = static_cast<std::byte>((v.year() % 10) + '0');
    q[4] = static_cast<std::byte>(((v.month() / 10) % 10u) + '0');
    q[5] = static_cast<std::byte>((v.month() % 10) + '0');
    q[6] = static_cast<std::byte>(((v.day() / 10) % 10) + '0');
    q[7] = static_cast<std::byte>((v.day() % 10) + '0');
    q[8] = std::byte{'-'};
    q[9] = static_cast<std::byte>(((v.hours() / 10) % 10) + '0');
    q[10] = static_cast<std::byte>((v.hours() % 10) + '0');
    q[11] = std::byte{':'};
    q[12] = static_cast<std::byte>(((v.minutes() / 10) % 10u) + '0');
    q[13] = static_cast<std::byte>((v.minutes() % 10) + '0');
    q[14] = std::byte{':'};
    q[15] = static_cast<std::byte>(((v.seconds() / 10) % 10) + '0');
    q[16] = static_cast<std::byte>((v.seconds() % 10) + '0');
    q[17] = std::byte{'.'};
    q[18] = static_cast<std::byte>(((v.milliseconds() / 100) % 10) + '0');
    q[19] = static_cast<std::byte>(((v.milliseconds() / 10) % 10) + '0');
    q[20] = static_cast<std::byte>((v.milliseconds() % 10) + '0');
}

} // namespace fix

#define FIX_STORE_CHRONO_(Buffer, Tag, Value, Size)                     \
    decltype(auto) fix_store_buf_ = (Buffer);                           \
    auto q = (fix_store_buf_).append(sizeof(#Tag) + Size);              \
    std::memcpy(q, #Tag "=", sizeof(#Tag));                             \
    q += sizeof(#Tag);                                                  \
    decltype(auto) v = (Value)

#define FIX_STORE_MONTH_YEAR(Buffer, Tag, Value)                        \
    do {                                                                \
        FIX_STORE_CHRONO_(Buffer, Tag, Value, 7);                       \
        q[0] = static_cast<std::byte>(((v.year() / 1000) % 10) + '0');  \
        q[1] = static_cast<std::byte>(((v.year() / 100) % 10) + '0');   \
        q[2] = static_cast<std::byte>(((v.year() / 10) % 10) + '0');    \
        q[3] = static_cast<std::byte>((v.year() % 10) + '0');           \
        q[4] = static_cast<std::byte>(((v.month() / 10) % 10u) + '0');  \
        q[5] = static_cast<std::byte>((v.month() % 10u) + '0');         \
        q[6] = std::byte{'\1'};                                         \
    } while (false)

#define FIX_STORE_DATE(Buffer, Tag, Value)                              \
    do {                                                                \
        FIX_STORE_CHRONO_(Buffer, Tag, Value, 9);                       \
        q[0] = static_cast<std::byte>(((v.year() / 1000) % 10) + '0');  \
        q[1] = static_cast<std::byte>(((v.year() / 100) % 10) + '0');   \
        q[2] = static_cast<std::byte>(((v.year() / 10) % 10) + '0');    \
        q[3] = static_cast<std::byte>((v.year() % 10) + '0');           \
        q[4] = static_cast<std::byte>(((v.month() / 10) % 10u) + '0');  \
        q[5] = static_cast<std::byte>((v.month() % 10) + '0');          \
        q[6] = static_cast<std::byte>(((v.day() / 10) % 10) + '0');     \
        q[7] = static_cast<std::byte>((v.day() % 10) + '0');            \
        q[8] = std::byte{'\1'};                                         \
    } while (false)

#define FIX_STORE_TIME(Buffer, Tag, Value)                                    \
    do {                                                                      \
        FIX_STORE_CHRONO_(Buffer, Tag, Value, 13);                            \
        q[0] = static_cast<std::byte>(((v.hours() / 10) % 10) + '0');         \
        q[1] = static_cast<std::byte>((v.hours() % 10) + '0');                \
        q[2] = std::byte{':'};                                                \
        q[3] = static_cast<std::byte>(((v.minutes() / 10) % 10u) + '0');      \
        q[4] = static_cast<std::byte>((v.minutes() % 10) + '0');              \
        q[5] = std::byte{':'};                                                \
        q[6] = static_cast<std::byte>(((v.seconds() / 10) % 10) + '0');       \
        q[7] = static_cast<std::byte>((v.seconds() % 10) + '0');              \
        q[8] = std::byte{'.'};                                                \
        q[9] = static_cast<std::byte>(((v.milliseconds() / 100) % 10) + '0'); \
        q[10] = static_cast<std::byte>(((v.milliseconds() / 10) % 10) + '0'); \
        q[11] = static_cast<std::byte>((v.milliseconds() % 10) + '0');        \
        q[12] = std::byte{'\1'};                                              \
    } while (false)

#define FIX_STORE_DATETIME(Buffer, Tag, Value)      \
    do {                                            \
        FIX_STORE_CHRONO_(Buffer, Tag, Value, 22);  \
        store(q, (v));                              \
        q[21] = std::byte{'\1'};                    \
    } while (false)

#endif // FIX_STORE_CHRONO_HH
