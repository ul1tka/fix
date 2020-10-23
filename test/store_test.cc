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

#include <fix/store.hh>
#include "buffer.hh"
#include "test.hh"

TEST(store, tag)
{
    static auto str = [](const std::byte* value, std::size_t size) noexcept {
        return std::string_view{reinterpret_cast<const char*>(value), size};
    };
    EXPECT_EQ("1=", str(FIX_STORE_TAG(1)));
    EXPECT_EQ("1234=", str(FIX_STORE_TAG(1234)));
}

TEST(store, bool)
{
    fix::test::buffer buffer;
    FIX_STORE_BOOL(buffer, 3846, false);
    EXPECT_EQ("3846=N\001", buffer.as_string());
    buffer.clear();
    FIX_STORE_BOOL(buffer, 3846, true);
    EXPECT_EQ("3846=Y\001", buffer.as_string());
}

TEST(store, char)
{
    fix::test::buffer buffer;
    FIX_STORE_CHAR(buffer, 98765, 'X');
    EXPECT_EQ("98765=X\001", buffer.as_string());
    buffer.clear();
    FIX_STORE_CHAR(buffer, 7346, '!');
    EXPECT_EQ("7346=!\001", buffer.as_string());
}

TEST(store, month_year)
{
    fix::test::buffer buffer;
    FIX_STORE_MONTH_YEAR(buffer, 68, fix::month_year(1986, 1));
    EXPECT_EQ("68=198601\001", buffer.as_string());
}

TEST(store, date)
{
    fix::test::buffer buffer;
    FIX_STORE_DATE(buffer, 2020, fix::date(1986, 1, 19));
    EXPECT_EQ("2020=19860119\001", buffer.as_string());
}

TEST(store, time)
{
    fix::test::buffer buffer;
    FIX_STORE_TIME(buffer, 74867, fix::time(19, 01, 5, 987));
    EXPECT_EQ("74867=19:01:05.987\001", buffer.as_string());
}

TEST(store, datetime)
{
    fix::test::buffer buffer;
    FIX_STORE_DATETIME(
        buffer, 464,
        fix::datetime(fix::date(2020, 1, 13), fix::time(21, 19, 1, 23))
    );
    EXPECT_EQ("464=20200113-21:19:01.023\001", buffer.as_string());
}
