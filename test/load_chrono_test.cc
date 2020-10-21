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

#include <fix/load/chrono.hh>
#include <fix/load/macros.hh>
#include "test.hh"

TEST(load, chrono)
{
    static const char msg[] =
        "1=202001\001"
        "2=20200111\001"
        "3=13:15:45\001"
        "4=18:01:34.999\001"
        "5=19860119-05:30:02\001"
        "6=19680330-12:34:56.123\001"
        "10=000\001"
        ;

    fix::month_year my{};
    fix::date d{};
    fix::time t{};
    fix::time tm{};
    fix::datetime dt{};
    fix::datetime dtm{};

    FIX_LOAD_BEGIN(reinterpret_cast<const std::byte*>(msg), 1, my);
    FIX_LOAD(2, d);
    FIX_LOAD(3, t);
    FIX_LOAD(4, tm);
    FIX_LOAD(5, dt);
    FIX_LOAD(6, dtm);
    FIX_LOAD_END();

    EXPECT_EQ(2020, my.year());
    EXPECT_EQ(1, my.month());

    EXPECT_EQ(2020, d.year());
    EXPECT_EQ(1, d.month());
    EXPECT_EQ(11, d.day());

    EXPECT_EQ(13, t.hours());
    EXPECT_EQ(15, t.minutes());
    EXPECT_EQ(45, t.seconds());
    EXPECT_EQ(0, t.milliseconds());

    EXPECT_EQ(18, tm.hours());
    EXPECT_EQ(1, tm.minutes());
    EXPECT_EQ(34, tm.seconds());
    EXPECT_EQ(999, tm.milliseconds());

    EXPECT_EQ(1986, dt.year());
    EXPECT_EQ(1, dt.month());
    EXPECT_EQ(19, dt.day());
    EXPECT_EQ(5, dt.hours());
    EXPECT_EQ(30, dt.minutes());
    EXPECT_EQ(2, dt.seconds());
    EXPECT_EQ(0, dt.milliseconds());

    EXPECT_EQ(1968, dtm.year());
    EXPECT_EQ(3, dtm.month());
    EXPECT_EQ(30, dtm.day());
    EXPECT_EQ(12, dtm.hours());
    EXPECT_EQ(34, dtm.minutes());
    EXPECT_EQ(56, dtm.seconds());
    EXPECT_EQ(123, dtm.milliseconds());
}
