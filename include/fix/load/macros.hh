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

#ifndef FIX_LOAD_MACROS_HH
#define FIX_LOAD_MACROS_HH

#include "../compiler.hh"
#include "tag.hh"
#include "skip.hh"

#define FIX_LOAD_BEGIN(Data, Tag, Field)            \
    FIX_LOAD_BEGIN_(Data, Tag)                      \
    case (Tag):                                     \
        std::tie(fix_tag_, fix_msg_ptr_) =          \
            ::fix::load_tag(                        \
                ::fix::load((Field), fix_msg_ptr_))

#define FIX_LOAD_BEGIN_SKIP(Data, Tag)                 \
    FIX_LOAD_BEGIN_(Data, Tag)                         \
    case (Tag):                                        \
        std::tie(fix_tag_, fix_msg_ptr_) =             \
            ::fix::load_tag(::fix::load(fix_msg_ptr_))

#define FIX_LOAD_BEGIN_(Data, Tag)         \
    {                                      \
        std::uint32_t fix_tag_;            \
        const std::byte* fix_msg_ptr_;     \
        std::tie(fix_tag_, fix_msg_ptr_) = \
            ::fix::load_tag((Data));       \
        if (FIX_LIKELY(fix_tag_ == (Tag))) \
            goto fix_load_first_tag_;      \
      fix_switch_on_tag_:                  \
        switch (fix_tag_) {                \
          fix_load_first_tag_:

#define FIX_LOAD_END()                                  \
        if (FIX_UNLIKELY(fix_tag_ != 10))               \
            goto fix_switch_on_tag_;                    \
        [[fallthrough]];                                \
        case 10:                                        \
            break;                                      \
        default:                                        \
          std::tie(fix_tag_, fix_msg_ptr_) =            \
            ::fix::load_tag(::fix::load(fix_msg_ptr_)); \
            goto fix_switch_on_tag_;                    \
        }                                               \
    } static_assert(true)

#define FIX_LOAD_CASE_(Tag)          \
    if (FIX_LIKELY(fix_tag_ != Tag)) \
        goto fix_switch_on_tag_;     \
    [[fallthrough]];                 \
    case Tag:

#define FIX_LOAD_SKIP(Tag)                          \
    FIX_LOAD_CASE_(Tag)                             \
    std::tie(fix_tag_, fix_msg_ptr_) =              \
        ::fix::load_tag(::fix::load(fix_msg_ptr_))

#define FIX_LOAD(Tag, Field)                    \
    FIX_LOAD_CASE_(Tag)                         \
    std::tie(fix_tag_, fix_msg_ptr_) =          \
        ::fix::load_tag(                        \
            ::fix::load((Field), fix_msg_ptr_))

#define FIX_LOAD_RAW(Tag1, Tag2, Field)                                 \
    FIX_LOAD_CASE_(Tag1)                                                \
    std::tie(fix_tag_, fix_msg_ptr_) =                                  \
        ::fix::load_tag(                                                \
            ::fix::load_raw((Field), fix_msg_ptr_, sizeof(#Tag2)))

#define FIX_LOAD_RAW_SKIP(Tag1, Tag2)                                   \
    FIX_LOAD_CASE_(Tag1)                                                \
    std::tie(fix_tag_, fix_msg_ptr_) =                                  \
        ::fix::load_tag(                                                \
            ::fix::load_raw(fix_msg_ptr_, sizeof(#Tag2)))

#define FIX_LOAD_SMALL(Tag, Field)                      \
    FIX_LOAD_CASE_(Tag)                                 \
    std::tie(fix_tag_, fix_msg_ptr_) =                  \
        ::fix::load_tag(                                \
            ::fix::load_small((Field), fix_msg_ptr_))

#endif // FIX_LOAD_MACROS_HH
