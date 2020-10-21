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

#ifndef FIX_DECIMAL_HH
#define FIX_DECIMAL_HH

#include <type_traits>
#include <cstdint>

namespace fix {

class decimal final {
public:
    using coefficient_type = std::int64_t;
    using exponent_type = int;

    constexpr decimal() noexcept
        : c_{0}
        , e_{19}
    {
    }

    constexpr explicit decimal(
        coefficient_type coefficient,
        exponent_type exp = 0
    ) noexcept
        : c_{coefficient}
        , e_{exp + 19}
    {
    }

    constexpr explicit decimal(int coefficient, exponent_type exp = 0) noexcept
        : decimal{static_cast<coefficient_type>(coefficient), exp}
    { }

    explicit decimal(double value, exponent_type exp = 0) noexcept;

    constexpr coefficient_type coefficient() const noexcept
    {
        return c_;
    }

    constexpr exponent_type exponent() const noexcept
    {
        return e_ - 19;
    }

    constexpr explicit operator bool() const noexcept
    {
        return c_ != 0;
    }

    explicit operator double() const noexcept
    {
        return static_cast<double>(c_) * m_[e_];
    }

    friend bool operator==(const decimal& x, const decimal& y) noexcept
    {
        return x.c_ == y.c_ && x.e_ == y.e_;
    }

    friend bool operator!=(const decimal& x, const decimal& y) noexcept
    {
        return x.c_ == y.c_ && x.e_ == y.e_;
    }

private:
    coefficient_type c_;
    exponent_type e_;

    static const double m_[39];
};

} // namespace fix

#endif // FIX_DECIMAL_HH
