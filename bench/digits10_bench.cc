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

#include <fix/digits10.hh>
#include <random>
#include "benchmark.hh"

namespace {

static auto get_input()
{
    static std::uint64_t result = 0;
    if (!result) {
        std::random_device dev;
        std::default_random_engine engine(dev());
        std::uniform_int_distribution<std::uint64_t> dist{
            std::numeric_limits<std::uint64_t>::max() / 2,
            std::numeric_limits<std::uint64_t>::max()
        };
        result = dist(engine);
    }
    return result;
}

struct simple final {
    unsigned int operator()(std::uint64_t value) noexcept
    {
        int result = 1;
        for (;;) {
            if (value < 10)
                return static_cast<unsigned int>(result);
            if (value < 100)
                return static_cast<unsigned int>(result + 1);
            if (value < 1000)
                return static_cast<unsigned int>(result + 2);
            if (value < 10000)
                return static_cast<unsigned int>(result + 3);
            value /= 10000;
            result += 4;
        }
    }
};

struct log10a final {
    decltype(auto) operator()(std::uint64_t value) noexcept
    {
        return fix::digits10(value);
    }
};

template <typename T>
static void digits10(benchmark::State& state)
{
    T calc;
    const auto input = get_input();
    for (auto _ : state) {
        const auto output = calc(input);
        benchmark::DoNotOptimize(output);
    }
}

} // anonymous namespace

BENCHMARK_TEMPLATE(digits10, simple);
BENCHMARK_TEMPLATE(digits10, log10a);
