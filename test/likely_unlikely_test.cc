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

#include "../fix/compiler.hh"
#include "test.hh"
#include <type_traits>

#ifndef FIX_LIKELY
# erorr "FIX_LIKELY macro is not defined"
#endif

#ifndef FIX_UNLIKELY
# erorr "FIX_UNLIKELY macro is not defined"
#endif

TEST(likely_unlikely, arguments)
{
    EXPECT_TRUE(FIX_LIKELY(std::is_same_v<int, int>));
    EXPECT_FALSE(FIX_LIKELY(std::is_same_v<int, float>));
    EXPECT_TRUE(FIX_UNLIKELY(std::is_same_v<int, int>));
    EXPECT_FALSE(FIX_UNLIKELY(std::is_same_v<int, float>));
}
