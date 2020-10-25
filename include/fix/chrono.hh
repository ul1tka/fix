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

#ifndef FIX_CHRONO_HH
#define FIX_CHRONO_HH

#include <chrono>
#include <cstdint>
#include <ctime>
#include <cassert>

namespace fix {

class month_year final {
    std::uint16_t y_;
    std::uint8_t m_;

public:
    month_year() noexcept = default;

    constexpr month_year(std::uint16_t year, std::uint8_t month) noexcept
        : y_{year}
        , m_{month}
    {
        assert(y_ <= 9999);
        assert(m_ > 0);
        assert(m_ <= 12);
    }

    constexpr auto year() const noexcept { return y_; }
    constexpr auto month() const noexcept { return m_; }
};

class date final {
    std::uint16_t y_;
    std::uint8_t m_;
    std::uint8_t d_;

public:
    date() noexcept = default;

    constexpr date(
        std::uint16_t year, std::uint8_t month, std::uint8_t day
    ) noexcept
        : y_{year}
        , m_{month}
        , d_{day}
    {
        assert(y_ <= 9999);
        assert(m_ > 0);
        assert(m_ <= 12);
        assert(d_ > 0);
        assert(d_ <= 31);
    }

    constexpr auto year() const noexcept { return y_; }
    constexpr auto month() const noexcept { return m_; }
    constexpr auto day() const noexcept { return d_; }
};

class time final {
    std::uint8_t h_ : 5;
    std::uint8_t m_ : 6;
    std::uint8_t s_ : 6;
    std::uint16_t ms_ : 10;

public:
    time() noexcept = default;

    constexpr time(
        std::uint8_t hours, std::uint8_t minutes, std::uint8_t seconds,
        std::uint16_t ms = 0
    ) noexcept
        : h_{hours}, m_{minutes}, s_{seconds}, ms_{ms}
    {
        assert(hours < 24);
        assert(minutes < 60);
        assert(seconds < 60);
        assert(ms < 1000);
    }

    constexpr auto hours() const noexcept { return h_; }
    constexpr auto minutes() const noexcept { return m_; }
    constexpr auto seconds() const noexcept { return s_; }
    constexpr auto milliseconds() const noexcept { return ms_; }
};

class datetime final {
    std::uint16_t year_ : 14;
    std::uint8_t month_ : 4;
    std::uint8_t day_ : 5;
    std::uint8_t hours_ : 5;
    std::uint8_t minutes_ : 6;
    std::uint8_t seconds_ : 6;
    std::uint16_t ms_ : 10;

public:
    datetime() noexcept = default;

    explicit datetime(std::time_t seconds, std::uint16_t ms = 0) noexcept;

    explicit datetime(const timespec& spec) noexcept
        : datetime{
            spec.tv_sec,
            static_cast<std::uint16_t>(spec.tv_nsec / 1000000)
          }
    {
    }

    constexpr explicit datetime(const date& date, const time& time) noexcept
        : year_{date.year()}, month_{date.month()}, day_{date.day()}
        , hours_{time.hours()}, minutes_{time.minutes()}
        , seconds_{time.seconds()}, ms_{time.milliseconds()}
    {
    }

    template <typename Rep, typename Period>
    explicit
    datetime(std::chrono::duration<Rep, Period> duration) noexcept
        : datetime{
            static_cast<std::time_t>(
                std::chrono::duration_cast<std::chrono::seconds>(
                    duration
                ).count()
            ),
            static_cast<std::uint16_t>(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    duration
                ).count() % 1000
            )
          }
    {
    }

    template <typename Clock, typename Duration>
    explicit
    datetime(std::chrono::time_point<Clock, Duration> time_point) noexcept
        : datetime{time_point.time_since_epoch()}
    {
    }

    constexpr auto year() const noexcept { return year_; }
    constexpr auto month() const noexcept { return month_; }
    constexpr auto day() const noexcept { return day_; }

    constexpr auto hours() const noexcept { return hours_; }
    constexpr auto minutes() const noexcept { return minutes_; }
    constexpr auto seconds() const noexcept { return seconds_; }

    constexpr auto milliseconds() const noexcept { return ms_; }
};

} // namespace fix

#endif // FIX_CHRONO_HH
