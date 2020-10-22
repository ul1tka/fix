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

#ifndef FIX_STORE_BYTE_HH
#define FIX_STORE_BYTE_HH

#define FIX_STORE_BYTE(Buffer, Tag, Value)                          \
    do {                                                            \
        decltype(auto) fix_store_buf_ = (Buffer);                   \
        static const char data[] = #Tag "= \001";                   \
        (fix_store_buf_).append(                                    \
            reinterpret_cast<const std::byte*>(data),               \
            sizeof(data) - 1                                        \
        )[sizeof(#Tag)] = Value;                                    \
    } while (false)

#endif // FIX_STORE_BYTE_HH
