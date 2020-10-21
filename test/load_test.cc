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

#include <fix/load.hh>
#include <limits>
#include "test.hh"

static const char message[] =
    "8=FIX.4.2\0019=0\00111=X\00112=1\00113=2\00114=Y\00115=N\001"
    "16=4294967295\001"
    "17=18446744073709551615\001"
    "18=2147483647\001"
    "19=9223372036854775807\001"
    "20=32767\001"
    "21=65535\001"
    "22=-32768\001"
    "23=-2147483648\001"
    "24=-9223372036854775808\001"
    "29=1\001"
    "10=000\001"
    ;

static const std::byte* test_message() noexcept
{
    return reinterpret_cast<const std::byte*>(message);
}

TEST(load, begin_end)
{
    std::string_view proto;
    FIX_LOAD_BEGIN(test_message(), 8, proto);
    FIX_LOAD_END();
    EXPECT_EQ("FIX.4.2", proto);
    EXPECT_EQ(message + 2, proto.data());
}

TEST(load, begin_end_miss)
{
    std::string_view length;
    FIX_LOAD_BEGIN(test_message(), 9, length);
    FIX_LOAD_END();
    EXPECT_EQ("0", length);
    EXPECT_EQ(message + 12, length.data());
}

TEST(load, begin_skip)
{
    FIX_LOAD_BEGIN_SKIP(test_message(), 8);
    FIX_LOAD_END();
}

TEST(load, begin_skip_miss)
{
    FIX_LOAD_BEGIN_SKIP(test_message(), 9);
    FIX_LOAD_END();
}

TEST(load, data)
{
    std::string_view p{};
    std::byte b{};
    char c{};
    unsigned char uc{};
    bool yes{};
    bool no{};
    std::uint32_t u32{};
    std::int32_t i32{};
    std::uint64_t u64{};
    std::int64_t i64{};
    std::uint16_t u16{};
    std::int16_t i16{};
    std::int16_t i16m{};
    std::int32_t i32m{};
    std::int64_t i64m{};
    std::optional<std::string_view> absent27;
    std::optional<std::uint32_t> absent28s;
    std::optional<std::uint32_t> o29;

    FIX_LOAD_BEGIN(test_message(), 8, p);
    FIX_LOAD_SKIP(9);
    FIX_LOAD(11, b);
    FIX_LOAD(12, c);
    FIX_LOAD(13, uc);
    FIX_LOAD(14, yes);
    FIX_LOAD(15, no);
    FIX_LOAD(16, u32);
    FIX_LOAD(17, u64);
    FIX_LOAD(18, i32);
    FIX_LOAD(19, i64);
    FIX_LOAD(20, i16);
    FIX_LOAD(21, u16);
    FIX_LOAD(22, i16m);
    FIX_LOAD(23, i32m);
    FIX_LOAD(24, i64m);
    FIX_LOAD(27, absent27);
    FIX_LOAD(29, o29);
    FIX_LOAD_END();

    EXPECT_EQ("FIX.4.2", p);
    EXPECT_EQ('X', std::to_integer<char>(b));
    EXPECT_EQ('1', c);
    EXPECT_EQ('2', uc);
    EXPECT_TRUE(yes);
    EXPECT_FALSE(no);
    EXPECT_EQ(std::numeric_limits<std::uint32_t>::max(), u32);
    EXPECT_EQ(std::numeric_limits<std::uint64_t>::max(), u64);
    EXPECT_EQ(std::numeric_limits<std::int32_t>::max(), i32);
    EXPECT_EQ(std::numeric_limits<std::int64_t>::max(), i64);
    EXPECT_EQ(std::numeric_limits<std::int16_t>::max(), i16);
    EXPECT_EQ(std::numeric_limits<std::uint16_t>::max(), u16);
    EXPECT_EQ(std::numeric_limits<std::int16_t>::min(), i16m);
    EXPECT_EQ(std::numeric_limits<std::int32_t>::min(), i32m);
    EXPECT_EQ(std::numeric_limits<std::int64_t>::min(), i64m);
    EXPECT_FALSE(absent27.has_value());
    EXPECT_FALSE(absent28s.has_value());
    EXPECT_EQ(1, o29.value_or(0));
}
