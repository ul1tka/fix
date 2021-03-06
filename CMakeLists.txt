#
# Copyright (C) 2020 Vlad Lazarenko <vlad@lazarenko.me>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#

cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(
  fix
  LANGUAGES CXX
  DESCRIPTION "Financial Information Exchange Engine and Tools"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include(FetchContent)

#
# Google Test (https://github.com/google/googletest)
#

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.10.0
)

FetchContent_GetProperties(googletest)

if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

#
# Google Benchmark (https://github.com/google/benchmark)
#
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.5.2
)

FetchContent_GetProperties(benchmark)

if(NOT benchmark_POPULATED)
  FetchContent_Populate(benchmark)
  set(
    BENCHMARK_ENABLE_TESTING OFF
    CACHE BOOL "Enable testing of the benchmark library."
    FORCE
  )
  set(
    BENCHMARK_ENABLE_GTEST_TESTS
    OFF
    CACHE BOOL "Enable building the unit tests which depend on gtest"
    FORCE
  )
  add_subdirectory(${benchmark_SOURCE_DIR} ${benchmark_BINARY_DIR})
endif()

get_target_property(INCLUDE_DIRECTORIES benchmark INTERFACE_INCLUDE_DIRECTORIES)
set_target_properties(benchmark PROPERTIES
  INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${INCLUDE_DIRECTORIES}")

enable_testing()

#
# Add support for coverage build
#
if(CMAKE_BUILD_TYPE MATCHES "Coverage")
  set(CMAKE_CXX_FLAGS "-g -O0 --coverage")
endif()

#
# FIX Engine Library
#

set(FIX_VERSION 1)
set(FIX_MAX_BODY_LENGTH_DIGITS 5)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/fix/detail)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/fix/detail/version.hh.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/fix/detail/version.hh
  @ONLY
  NEWLINE_STYLE UNIX
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/fix/config.hh.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/fix/config.hh
  @ONLY
  NEWLINE_STYLE UNIX
)

add_library(
  fix-engine
  src/version.cc
  src/chrono.cc
  src/decimal.cc
  src/digits10.cc
  src/store/sending_time.cc
  src/store/context.cc
)

target_include_directories(
  fix-engine
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_compile_options(
  fix-engine
  PUBLIC
    -Wall
    -Wextra
    -Werror
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-padded
    -Wno-gnu-zero-variadic-macro-arguments
    -fno-exceptions
    -fno-rtti
    -ffast-math
    -fvisibility=hidden
    -mtune=native
    -march=native
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(
    fix-engine
    PUBLIC
      -Weverything
  )
endif()

add_executable(
  test-fix-engine
  test/version_test.cc
  test/likely_unlikely_test.cc
  test/capacitor_test.cc
  test/decimal_test.cc
  test/digits10_test.cc
  test/checksum_test.cc
  test/load_chrono_test.cc
  test/load_number_test.cc
  test/load_decimal_test.cc
  test/load_raw_test.cc
  test/load_test.cc
  test/store_context_test.cc
  test/store_test.cc
)

target_link_libraries(
  test-fix-engine
  PRIVATE
    gtest
    gmock
    gtest_main
    fix-engine
)

target_compile_options(
  test-fix-engine
  PRIVATE
    -Wno-global-constructors
)

add_executable(
  bench-fix-engine
  bench/main.cc
  bench/checksum_bench.cc
  bench/digits10_bench.cc
  bench/timestamp_bench.cc
)

target_link_libraries(
  bench-fix-engine
  PRIVATE
    benchmark
    fix-engine
)

target_compile_options(
  bench-fix-engine
  PRIVATE
    -Wno-global-constructors
)

include(GoogleTest)

gtest_discover_tests(
  test-fix-engine
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DISCOVERY_MODE POST_BUILD
)
