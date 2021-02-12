//
// Copyright (C) 2012-2021 Vlad Lazarenko <vlad@lazarenko.me>
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; version 2 of the License.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 51
// Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//

#ifndef FIX_VERSION_HH
#define FIX_VERSION_HH

#include "compiler.hh"
#include <fix/internal/version.hh>

#include <compare>
#include <iosfwd>

#define FIX_VERSION_MAJOR (FIX_VERSION / 1000000)
#define FIX_VERSION_MINOR (FIX_VERSION / 1000 % 1000)
#define FIX_VERSION_PATCH (FIX_VERSION % 1000)

namespace fix {

class Version final {
        unsigned int value_;

    public:
        constexpr explicit Version(unsigned int value) noexcept : value_{value} {}

        [[nodiscard]] constexpr unsigned int getValue() const noexcept { return value_; }

        [[nodiscard]] constexpr unsigned int getMajor() const noexcept { return value_ / 1000000; }

        [[nodiscard]] constexpr unsigned int getMinor() const noexcept { return value_ / 1000 % 1000; }

        [[nodiscard]] constexpr unsigned int getPatch() const noexcept { return value_ % 1000; }

        [[nodiscard]] constexpr auto operator<=>(const Version& other) const noexcept = default;
};

constexpr Version getVersion() noexcept { return Version{FIX_VERSION}; }

FIX_COLD Version getRuntimeVersion() noexcept;

FIX_COLD std::ostream& operator<<(std::ostream& out, const Version& version);

} // namespace fix

#endif
