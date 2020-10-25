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

#include <fix/store/header.hh>
#include <fix/store/string.hh>
#include <fix/config.hh>
#include "buffer.hh"
#include "test.hh"

static std::string make_raw_header(
    std::string_view proto,
    std::string_view sender, std::string_view target,
    std::string_view type
) {
    std::string data{"8="};
    data += proto;
    data += "\0019=";
    data.append(fix::max_body_length_digits(), ' ');
    data += "\00135=";
    data += type;
    data += "\00149=";
    data += sender;
    data += "\00156=";
    data += target;
    data += "\001";
    return data;
}

static std::string make_header(
    std::string_view proto,
    std::string_view sender, std::string_view target,
    std::string_view type,
    std::uint64_t sequence
) {
    return make_raw_header(proto, sender, target, type)
        .append("34=").append(std::to_string(sequence)).append(1, '\001');
}

TEST(store, header_simple)
{
    fix::test::buffer buffer;
    fix::header ctx{"FIX.4.1", "Sender", "Target"};
    ctx.store(buffer, '0', 12345);
    EXPECT_EQ(make_header("FIX.4.1", "Sender", "Target", "0", 12345),
              buffer.as_string());
}

TEST(store, header_long_type)
{
    fix::test::buffer buffer;
    fix::header ctx{"FIX.4.2", "tx", "rx"};
    ctx.store(buffer, "XYZ", 3489);
    EXPECT_EQ(make_header("FIX.4.2", "tx", "rx", "XYZ", 3489),
              buffer.as_string());
}

TEST(store, header_custom_data)
{
    fix::test::buffer buffer;
    fix::header ctx{"FIX.4.3", "From", "To"};
    ctx.append("115=Client\001");
    ctx.append("128=Exchange\001");
    ctx.store(buffer, 'D', 872345);
    EXPECT_EQ((make_raw_header("FIX.4.3", "From", "To", "D")
               .append("115=Client\001128=Exchange\00134=872345\001")),
              buffer.as_string());
}

TEST(store, header_as_buffer)
{
    fix::header header{"FIX.4.2", "Broker", "Exchange"};

    FIX_STORE_STRING(header, 115, std::string_view{"Trader"});
    FIX_STORE_STRING(header, 128, std::string_view{"DarkPool"});
    fix::test::buffer buffer;
    FIX_STORE_BEGIN(buffer, header, '!', 12345);
    EXPECT_EQ((make_raw_header("FIX.4.2", "Broker", "Exchange", "!")
               .append("115=Trader\001128=DarkPool\00134=12345\001")),
              buffer.as_string());

    /// @todo Check sequence serialization...
}
