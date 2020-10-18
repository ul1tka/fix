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

#ifndef FIX_VERSION_HH
#define FIX_VERSION_HH

#include <fix/detail/version.hh>
#include <compare>
#include <iosfwd>

#define FIX_VERSION_MAJOR (FIX_VERSION / 1000000)
#define FIX_VERSION_MINOR (FIX_VERSION / 1000 % 1000)
#define FIX_VERSION_PATCH (FIX_VERSION % 1000)

namespace fix {

class version final {
    unsigned int value_;

public:
    constexpr explicit version(unsigned int value) noexcept
        : value_{value}
    { }

    constexpr unsigned int value() const noexcept {
        return value_;
    }

    constexpr unsigned int major() const noexcept {
        return value_ / 1000000;
    }

    constexpr unsigned int minor() const noexcept {
        return value_ / 1000 % 1000;
    }

    constexpr unsigned int patch() const noexcept {
        return value_ % 1000;
    }

    constexpr auto operator<=>(const version& other) const noexcept = default;
};

constexpr version get_version() noexcept
{
    return version{FIX_VERSION};
}

version get_runtime_version() noexcept;

std::ostream& operator<<(std::ostream& out, const version& version);

} // namespace fix

#endif // FIX_VERSION_HH
