#!/bin/sh
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

set -ex

mkdir build && cd build

export MAKEFLAGS="-j"

command -v ninja >/dev/null && CMAKE_FLAGS="-GNinja"

test -n "${CC}" && CMAKE_FLAGS="${CMAKE_FLAGS} -DCMAKE_C_COMPIER=${CC}"

test -n "${CXX}" && CMAKE_FLAGS="${CMAKE_FLAGS} -DCMAKE_CXX_COMPIER=${CXX}"

BUILD_TYPE=${BUILD_TYPE:-Debug}

cmake \
    ${CMAKE_FLAGS} \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DCMAKE_VERBOSE_MAKEFILE=ON \
    ..

cmake --build .

ctest \
    --repeat-until-fail 2 \
    --timeout 600 \
    --parallel \
    --schedule-random \
    --output-on-failure
