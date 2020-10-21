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

#include <fix/load/decimal.hh>
#include "test.hh"

#define TEST_LOAD(value)                                        \
    do {                                                        \
        fix::decimal decimal;                                   \
        static const char input[] = #value "\001";              \
        EXPECT_EQ(                                              \
            reinterpret_cast<const std::byte*>(                 \
                input + sizeof(input) - 1                       \
            ),                                                  \
            fix::load(                                          \
                decimal,                                        \
                reinterpret_cast<const std::byte*>(input)       \
            )                                                   \
        );                                                      \
        EXPECT_DOUBLE_EQ(value, static_cast<double>(decimal));  \
    } while (false)

TEST(load, decimal)
{
    TEST_LOAD(0);
    TEST_LOAD(-0);
    TEST_LOAD(1);
    TEST_LOAD(-1);
    TEST_LOAD(1986);
    TEST_LOAD(-1986);
    TEST_LOAD(-12345.6789);
    TEST_LOAD(98.7654321);
    TEST_LOAD(19.86);
    TEST_LOAD(.001);
    TEST_LOAD(-.02);
}
