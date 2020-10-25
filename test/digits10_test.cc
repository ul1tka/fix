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

#include <fix/digits10.hh>
#include <limits>
#include "test.hh"

TEST(digits10, basic)
{
    EXPECT_EQ(1, fix::digits10(0));
    EXPECT_EQ(1, fix::digits10(1));
    EXPECT_EQ(1, fix::digits10(9));
    EXPECT_EQ(2, fix::digits10(10));
    EXPECT_EQ(2, fix::digits10(99));
    EXPECT_EQ(3, fix::digits10(100));
    EXPECT_EQ(3, fix::digits10(999));
    EXPECT_EQ(
        std::numeric_limits<std::uint64_t>::digits10 + 1,
        fix::digits10(std::numeric_limits<std::uint64_t>::max())
    );
}
