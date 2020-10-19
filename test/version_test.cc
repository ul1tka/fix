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

#include <fix/version.hh>
#include <string>
#include <sstream>
#include "test.hh"

TEST(version, parts)
{
    constexpr fix::version version{123456789};
    EXPECT_EQ(123456789, version.value());
    EXPECT_EQ(123, version.major());
    EXPECT_EQ(456, version.minor());
    EXPECT_EQ(789, version.patch());
}

TEST(version, compile_time_matches_runtime)
{
    EXPECT_EQ(fix::get_version(), fix::get_runtime_version());
}

TEST(version, version_matches_macros)
{
    EXPECT_EQ(FIX_VERSION, fix::get_version().value());
    EXPECT_EQ(FIX_VERSION_MAJOR, fix::get_version().major());
    EXPECT_EQ(FIX_VERSION_MINOR, fix::get_version().minor());
    EXPECT_EQ(FIX_VERSION_PATCH, fix::get_version().patch());
}

TEST(version, ostream)
{
    for (auto version : { fix::get_version(), fix::get_runtime_version() }) {
        std::ostringstream stream;
        stream << version;
        EXPECT_EQ(std::to_string(version.major()) + '.' +
                  std::to_string(version.minor()) + '.' +
                  std::to_string(version.patch()),
                  std::move(stream).str());
    }
}
