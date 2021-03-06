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

#ifndef FIX_STORE_MACROS_HH
#define FIX_STORE_MACROS_HH

#include <cstddef>

#define FIX_STRINGIFY(X) #X

#define FIX_STORE_TAG(Tag)                                       \
    reinterpret_cast<const std::byte*>(FIX_STRINGIFY(Tag) "="),  \
    sizeof(FIX_STRINGIFY(Tag))

#define FIX_STORE(Buffer, Tag, Value)                   \
    do {                                                \
        using namespace fix;                            \
        store((Buffer), FIX_STORE_TAG(Tag), (Value));   \
    } while (false)

#endif // FIX_STORE_MACROS_HH
