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

#include <fix/load/raw.hh>
#include <fix/load/macros.hh>
#include "test.hh"

TEST(load, raw_data)
{
    static const char raw_msg[] =
        "8=FIX.4.2\0019=0\001"
        "30=3\00131=XY\001\001"
        "32=5\001123=\001\001\001\001\001\001"
        "33=5\001123=\001\001\001\001\001\001"
        "10=000\001"
        ;
    std::string_view raw30x31;
    std::string_view raw33x9999;

    FIX_LOAD_BEGIN_SKIP(reinterpret_cast<const std::byte*>(raw_msg), 8);
    FIX_LOAD_RAW(30, 31, raw30x31);
    FIX_LOAD_RAW_SKIP(32, 123);
    FIX_LOAD_RAW(33, 123, raw33x9999);
    FIX_LOAD_END();

    EXPECT_EQ("XY\001", raw30x31);
    EXPECT_EQ("\001\001\001\001\001", raw33x9999);
}
