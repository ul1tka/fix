//
// Copyright (C) 2012-2021 Vlad Lazarenko <vlad@lazarenko.me>
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; version 2 of the License.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 51
// Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//

#include "version.hh"
#include <gtest/gtest.h>
#include <string>

TEST(Version, Parts) {
        constexpr fix::Version version{123456789};
        EXPECT_EQ(123456789, version.getValue());
        EXPECT_EQ(123, version.getMajor());
        EXPECT_EQ(456, version.getMinor());
        EXPECT_EQ(789, version.getPatch());
}

TEST(Version, CompileTimeMatchesRuntime) { EXPECT_EQ(fix::getVersion(), fix::getRuntimeVersion()); }

TEST(Version, VersionMatchesMacros) {
        EXPECT_EQ(FIX_VERSION, fix::getVersion().getValue());
        EXPECT_EQ(FIX_VERSION_MAJOR, fix::getVersion().getMajor());
        EXPECT_EQ(FIX_VERSION_MINOR, fix::getVersion().getMinor());
        EXPECT_EQ(FIX_VERSION_PATCH, fix::getVersion().getPatch());
}

TEST(Version, Output) {
        for (auto version : {fix::getVersion(), fix::getRuntimeVersion()}) {
                std::ostringstream stream;
                stream << version;
                EXPECT_EQ(std::to_string(version.getMajor()) + '.' + std::to_string(version.getMinor()) + '.' +
                              std::to_string(version.getPatch()),
                          std::move(stream).str());
        }
}
