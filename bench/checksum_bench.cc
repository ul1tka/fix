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

#include <fix/checksum.hh>
#include <random>
#include "benchmark.hh"

namespace {

struct native final {
    static decltype(auto)
    checksum(const std::byte* data, std::size_t size) noexcept
    {
        return fix::checksum(data, size);
    }
};

struct simple final {
    static auto checksum(const std::byte* data, std::size_t size) noexcept
    {
        unsigned int s = 0;
        for (std::size_t i = 0; i < size; ++i)
            s += std::to_integer<unsigned int>(data[i]);
        return static_cast<unsigned int>(s % 256);
    }
};

struct unrolled_2 final {
    static auto checksum(const std::byte* data, std::size_t size) noexcept
    {
        unsigned int s0{};
        unsigned int s1{};

        const auto end = data + size;

        for (const auto e = end - (size % 2); data < e; data += 2) {
            s0 += std::to_integer<unsigned int>(data[0]);
            s1 += std::to_integer<unsigned int>(data[1]);
        }

        while (data < end) {
            s0 += std::to_integer<unsigned int>(*data++);
        }

        return static_cast<unsigned int>((s0 + s1) % 256);
    }
};

struct unrolled_4 final {
    static auto checksum(const std::byte* data, std::size_t size) noexcept
    {
        unsigned int s0{};
        unsigned int s1{};
        unsigned int s2{};
        unsigned int s3{};

        const auto end = data + size;

        for (const auto e = end - (size % 4); data < e; data += 4) {
            s0 += std::to_integer<unsigned int>(data[0]);
            s1 += std::to_integer<unsigned int>(data[1]);
            s2 += std::to_integer<unsigned int>(data[2]);
            s3 += std::to_integer<unsigned int>(data[3]);
        }

        while (data < end) {
            s0 += std::to_integer<unsigned int>(*data++);
        }

        return static_cast<unsigned int>((s0 + s1 + s2 + s3) % 256);
    }
};

template <typename T>
static void checksum(benchmark::State& state)
{
    auto generate_message = [&]{
        std::random_device random_device;
        std::default_random_engine random_engine(random_device());
        std::uniform_int_distribution<int> ascii_dist('@', '~');
        const auto size = static_cast<std::size_t>(state.range(0));
        std::vector<std::byte> data;
        data.resize(size);
        for (auto& x : data) {
            x = std::byte(ascii_dist(random_engine));
        }
        return data;
    };

    const auto message = generate_message();

    if (message.empty()) {
        state.SkipWithError("Failed to generate data");
        return;
    }

    const auto data = &message[0];
    const auto size = message.size();

    const auto checksum = fix::checksum(data, size);

    for (auto _ : state) {
        const auto value = T::checksum(data, size);
        benchmark::DoNotOptimize(value);
        if (value != checksum) {
            state.SkipWithError("Failed to calculate checksum correctly");
            break;
        }
    }
}

} // anonymous namespace

#define CHECKSUM_BENCHMARK(type) \
    BENCHMARK_TEMPLATE(checksum, type) \
    ->Arg(256)->Arg(512)->Arg(2048)

CHECKSUM_BENCHMARK(native);
CHECKSUM_BENCHMARK(simple);
CHECKSUM_BENCHMARK(unrolled_2);
CHECKSUM_BENCHMARK(unrolled_4);
