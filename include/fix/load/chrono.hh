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

#ifndef FIX_LOAD_CHRONO_HH
#define FIX_LOAD_CHRONO_HH

#include "../chrono.hh"
#include <cstddef>

namespace fix {

inline const std::byte*
load(month_year& dst, const std::byte* data) noexcept
{
    dst = month_year{
        static_cast<std::uint16_t>(
            (std::to_integer<int>(data[0]) - '0') * 1000 +
            (std::to_integer<int>(data[1]) - '0') * 100 +
            (std::to_integer<int>(data[2]) - '0') * 10 +
            (std::to_integer<int>(data[3]) - '0')
        ),
        static_cast<std::uint8_t>(
            (std::to_integer<int>(data[4]) - '0') * 10 +
            (std::to_integer<int>(data[5]) - '0')
        )
    };
    assert(data[6] == std::byte{'\1'});
    return data + 7;
}

inline const std::byte*
load(date& dst, const std::byte* data) noexcept
{
    dst = date{
        static_cast<std::uint16_t>(
            (std::to_integer<int>(data[0]) - '0') * 1000 +
            (std::to_integer<int>(data[1]) - '0') * 100 +
            (std::to_integer<int>(data[2]) - '0') * 10 +
            (std::to_integer<int>(data[3]) - '0')
        ),
        static_cast<std::uint8_t>(
            (std::to_integer<int>(data[4]) - '0') * 10 +
            (std::to_integer<int>(data[5]) - '0')
        ),
        static_cast<std::uint8_t>(
            (std::to_integer<int>(data[6]) - '0') * 10 +
            (std::to_integer<int>(data[7]) - '0')
        )
    };
    assert(data[8] == std::byte{'\1'});
    return data + 9;
}

inline const std::byte*
load(time& dst, const std::byte* data) noexcept
{
    const auto has_ms = data[8] == std::byte{'.'};
    dst = time{
        static_cast<std::uint8_t>(
            (std::to_integer<int>(data[0]) - '0') * 10 +
            (std::to_integer<int>(data[1]) - '0')
        ),
        static_cast<std::uint8_t>(
            (std::to_integer<int>(data[3]) - '0') * 10 +
            (std::to_integer<int>(data[4]) - '0')
        ),
        static_cast<std::uint8_t>(
            (std::to_integer<int>(data[6]) - '0') * 10 +
            (std::to_integer<int>(data[7]) - '0')
        ),
        static_cast<std::uint16_t>(
            has_ms ?
            (
                (std::to_integer<int>(data[9]) - '0') * 100 +
                (std::to_integer<int>(data[10]) - '0') * 10 +
                (std::to_integer<int>(data[11]) - '0')
            ) : 0
        )
    };
    assert(data[2] == std::byte{':'});
    assert(data[5] == std::byte{':'});
    assert(data[has_ms ? 12 : 8] == std::byte{'\1'});
    return data + (has_ms ? 13 : 9);
}

inline const std::byte*
load(datetime& dst, const std::byte* data) noexcept
{
    const auto has_ms = data[17] == std::byte{'.'};
    dst = datetime{
        date{
            static_cast<std::uint16_t>(
                (std::to_integer<int>(data[0]) - '0') * 1000 +
                (std::to_integer<int>(data[1]) - '0') * 100 +
                (std::to_integer<int>(data[2]) - '0') * 10 +
                (std::to_integer<int>(data[3]) - '0')
            ),
            static_cast<std::uint8_t>(
                (std::to_integer<int>(data[4]) - '0') * 10 +
                (std::to_integer<int>(data[5]) - '0')
            ),
            static_cast<std::uint8_t>(
                (std::to_integer<int>(data[6]) - '0') * 10 +
                (std::to_integer<int>(data[7]) - '0')
            )
        },
        time{
            static_cast<std::uint8_t>(
                (std::to_integer<int>(data[9]) - '0') * 10 +
                (std::to_integer<int>(data[10]) - '0')
            ),
            static_cast<std::uint8_t>(
                (std::to_integer<int>(data[12]) - '0') * 10 +
                (std::to_integer<int>(data[13]) - '0')
            ),
            static_cast<std::uint8_t>(
                (std::to_integer<int>(data[15]) - '0') * 10 +
                (std::to_integer<int>(data[16]) - '0')
            ),
            static_cast<std::uint16_t>(
                has_ms ?
                (
                    (std::to_integer<int>(data[18]) - '0') * 100 +
                    (std::to_integer<int>(data[19]) - '0') * 10 +
                    (std::to_integer<int>(data[20]) - '0')
                ) : 0
            )
        }
    };
    assert(data[8] == std::byte{'-'});
    assert(data[has_ms ? 21 : 17] == std::byte{'\1'});
    return data + (has_ms ? 22 : 18);
}

} // namespace fix

#endif // FIX_LOAD_CHRONO_HH
