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

#include <fix/store/chrono.hh>
#include "benchmark.hh"

static void timestamp_store(benchmark::State& state)
{
    std::byte data[256] = {};
    const fix::datetime timestamp{std::chrono::system_clock::now()};

    std::memcpy(data, "Hello", 5);
    std::memcpy(data + 26, "there", 6);

    for (auto _ : state) {
        fix::store(data + 5, timestamp);
        benchmark::DoNotOptimize(data);
        benchmark::ClobberMemory();
    }
}

static void timestamp_cache(benchmark::State& state)
{
    std::byte data[256] = {};
    const fix::datetime timestamp{std::chrono::system_clock::now()};
    std::byte serialized_time[21];
    fix::store(serialized_time, timestamp);
    std::memcpy(data, "Hello", 5);

    for (auto _ : state) {
        std::memcpy(data + 5, serialized_time, sizeof(serialized_time));
        std::memcpy(data + 26, "there", 6);
        benchmark::DoNotOptimize(data);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(timestamp_store);
BENCHMARK(timestamp_cache);
