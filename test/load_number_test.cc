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

#include <fix/load/number.hh>
#include <limits>
#include <string>
#include <cstdio>
#include "test.hh"

namespace {

template <typename T>
class load_number : public testing::Test {
public:
    using Test::Test;
};

using number_types = testing::Types<
    std::int16_t, std::uint16_t,
    std::int32_t, std::uint32_t,
    std::int64_t, std::uint64_t
>;

TYPED_TEST_SUITE(load_number, number_types);

} // anonymous namespace

TYPED_TEST(load_number, simple)
{
    static const TypeParam values[] = {
        0, 1, 2, 9, 1986, 2020,
        std::numeric_limits<TypeParam>::min(),
        std::numeric_limits<TypeParam>::min() + 1,
        std::numeric_limits<TypeParam>::max(),
        std::numeric_limits<TypeParam>::max() - 1
    };
    for (auto x : values) {
        const auto s = std::to_string(x) + '\001';
        TypeParam y{};
        auto e = fix::load(y, reinterpret_cast<const std::byte*>(s.c_str()));
        EXPECT_EQ(static_cast<const void*>(e),
                  static_cast<const void*>(&(*s.rbegin()) + 1));
        EXPECT_EQ(x, y);
    }
}
