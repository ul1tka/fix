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

#include <fix/decimal.hh>
#include "test.hh"

#define EXPECT_ZERO(...)                            \
    do {                                            \
        EXPECT_EQ(0, (__VA_ARGS__).coefficient());  \
        EXPECT_EQ(0, (__VA_ARGS__).exponent());     \
        EXPECT_FALSE((__VA_ARGS__));                \
        EXPECT_TRUE(!(__VA_ARGS__));                \
    } while (false)

TEST(decimal, zero)
{
    EXPECT_ZERO(fix::decimal{});
    EXPECT_ZERO(fix::decimal{0});
    EXPECT_ZERO(fix::decimal{0, 0});
}

#undef EXPECT_ZERO

TEST(decimal, float_as_is)
{
    fix::decimal one{1.0};
    EXPECT_EQ(1, one.coefficient());
    EXPECT_EQ(0, one.exponent());
    EXPECT_TRUE(one);
}

TEST(decimal, exponent_limits)
{
    EXPECT_DOUBLE_EQ(1 / 1e19, static_cast<double>(fix::decimal{1, -19}));
    EXPECT_DOUBLE_EQ(1 * 1e19, static_cast<double>(fix::decimal{1, 19}));
}

TEST(decimal, from_double)
{
    fix::decimal dec{1234.5678, -4};
    EXPECT_EQ(12345678, dec.coefficient());
    EXPECT_EQ(-4, dec.exponent());
    EXPECT_DOUBLE_EQ(1234.5678, static_cast<double>(dec));
}
