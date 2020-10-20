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

#include <fix/capacitor.hh>
#include <algorithm>
#include <string>
#include <vector>
#include "test.hh"

namespace {

using messages = std::vector<std::string>;

static messages parse(std::string_view data)
{
    messages result;
    for (std::size_t stride = 1; stride <= data.size(); ++stride) {
        messages list;
        fix::capacitor cap;
        auto begin = reinterpret_cast<const std::byte*>(&data[0]);
        auto remainder = data.size();
        do {
            const auto size = std::min(remainder, stride);
            cap.add(
                begin, begin + size,
                [&](const std::byte* msg_begin, const std::byte* msg_end) {
                    ASSERT_LT(msg_begin, msg_end);
                    list.emplace_back(
                        reinterpret_cast<const char*>(msg_begin),
                        reinterpret_cast<const char*>(msg_end)
                    );
                }
            );
            begin += size;
            remainder -= size;
        } while (remainder > 0);
        if (stride > 1) {
            EXPECT_EQ(result, list);
        } else {
            result = std::move(list);
        }
    }
    return result;
}

} // anonymous namespace

TEST(capacitor, bad_range)
{
    fix::capacitor cap;
    static const std::byte data[] = { std::byte{'1'}, std::byte{'2'} };
    const auto unexpected = [](auto...) {
        ADD_FAILURE() << "Unexpected message";
    };
    cap.add(&data[1], &data[0], unexpected);
    cap.add(&data[0], &data[0], unexpected);
    cap.add(nullptr, nullptr, unexpected);
}

TEST(capacitor, simple_parsing)
{
    EXPECT_EQ(messages{}, parse("==\001123456"));
    EXPECT_EQ(messages{"1234567"}, parse("==\0011234567"));
    EXPECT_EQ(messages{"10=000\1"}, parse("8=FIX.4.0\0019=\00110=000\1"));
    EXPECT_EQ(messages{"10=000\1"}, parse("8=FIX.4.0\0019=0\00110=000\1"));
    EXPECT_EQ(
        messages{"X10=000\1"},
        parse("8=FIX.4.0\0019=1\001X10=000\18=FIX.4.0\0019=2\001XY10=000")
    );
    EXPECT_EQ(
        (messages{"X10=000\1", "XY10=000\1"}),
        parse("8=FIX.4.0\0019=1\001X10=000\18=FIX.4.0\0019=2\001XY10=000\1")
    );
    EXPECT_EQ(
        (messages{
            "35=A\00149=SERVER\00156=CLIENT\00134=177\001"
            "52=20090107-18:15:16\00198=0\001108=30\00110=062\001",
            "35=B\00149=SERVER\00156=CLIENT\00134=177\001"
            "52=20090107-18:15:16\00198=0\001108=30\00110=062\001"
        }),
        parse(
            "8=FIX.4.2\0019=65\00135=A\00149=SERVER\00156=CLIENT\001"
            "34=177\00152=20090107-18:15:16\00198=0\001108=30\00110=062\001"
            "8=FIX.4.2\0019=65\00135=B\00149=SERVER\00156=CLIENT\001"
            "34=177\00152=20090107-18:15:16\00198=0\001108=30\00110=062\001"
            "8=FIX.4.2\0019=65\00135=C\00149=SERVER\00156=CLIENT\001"
            "34=177\00152=20090107-18:15:16\00198=0\001108=30\00110=062"
        )
    );
}
