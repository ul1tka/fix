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

#include <fix/store/context.hh>
#include <fix/store/string.hh>
#include <fix/store/chrono.hh>
#include <fix/config.hh>

#include "buffer.hh"
#include "test.hh"

#include <string>
#include <random>

namespace {

class store_context : public testing::Test {
public:
    store_context() = default;

    ~store_context() override;

    void SetUp() override;

protected:
    std::uint64_t random_sequence();

    std::string context(std::string_view type, std::uint64_t sequence,
                        std::string_view custom_data = std::string_view{});

    std::random_device dev_;
    std::default_random_engine eng_{dev_()};

    std::string proto_;
    std::string sender_;
    std::string target_;

    fix::context context_;

    fix::datetime time_;
    fix::sending_time send_time_;

    fix::test::buffer buffer_;
};

store_context::~store_context()
{
}

void store_context::SetUp()
{
    proto_.assign("FIX.4.");
    proto_.append(1, std::uniform_int_distribution<char>{'1', '4'}(eng_));

    sender_.clear();
    target_.clear();

    std::uniform_int_distribution<char> ascii_dist{'A', 'Z'};

    for (int i = 0; i < 6; ++i) {
        sender_.append(1, ascii_dist(eng_));
        target_.append(1, ascii_dist(eng_));
    }

    context_ = fix::context{proto_, sender_, target_};

    time_ = fix::datetime{std::chrono::system_clock::now()};
    send_time_.set(time_);
}

std::uint64_t store_context::random_sequence()
{
    std::uniform_int_distribution<std::uint64_t> dist{100, 99999999};
    return dist(eng_);
}

std::string store_context::context(
    std::string_view type, std::uint64_t sequence,
    std::string_view custom_data
) {
    std::string data{"8="};
    data += proto_;
    data += "\0019=";
    data.append(fix::max_body_length_digits(), ' ');
    data += "\00135=";
    data += type;
    data += "\00149=";
    data += sender_;
    data += "\00156=";
    data += target_;
    data += "\001";

    data += custom_data;

    data += "52=";

    std::byte time_bytes[21];
    fix::store(time_bytes, time_);
    data.append(reinterpret_cast<const char*>(time_bytes),
                sizeof(time_bytes));
    data.append(1, '\001');
    data += "34=";
    data += std::to_string(sequence);
    data += '\001';
    return data;
}

} // anonymous namespace

TEST_F(store_context, simple)
{
    const auto seq = random_sequence();
    context_.store_head(buffer_, '0', send_time_, seq);
    EXPECT_EQ(context("0", seq), buffer_.as_string());
}

TEST_F(store_context, long_type)
{
    const auto seq = random_sequence();
    context_.store_head(buffer_, "XYZ", send_time_, seq);
    EXPECT_EQ(context("XYZ", seq), buffer_.as_string());
}

TEST_F(store_context, custom_data)
{
    const auto seq = random_sequence();
    FIX_STORE_STRING(context_, 115, std::string_view{"Trader"});
    FIX_STORE_STRING(context_, 128, std::string_view{"DarkPool"});
    context_.store_head(buffer_, 'D', send_time_, seq);
    EXPECT_EQ(context("D", seq, "115=Trader\001128=DarkPool\001"),
              buffer_.as_string());
}

TEST(store, context_seal)
{
    fix::test::buffer buffer;
    fix::context context{"FIX.4.2", "Trader", "DarkPool"};
    fix::sending_time send_time;
    send_time.set(fix::datetime{{1986, 1, 19}, {5, 30, 1, 3}});
    context.store_head(buffer, 'A', send_time, 177);
    context.store_tail(buffer);
    EXPECT_EQ("8=FIX.4.2\0019=00059\00135=A\00149=Trader\00156=DarkPool\001"
              "52=19860119-05:30:01.003\00134=177\00110=100\001",
              buffer.as_string());
}
