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
#include "test.hh"

TEST(checksum, basic)
{
    static const char message[] =
        "8=FIX.4.2\0019=178\00135=8\00149=PHLX\00156=PERS\001"
        "52=20071123-05:30:00.000\00111=ATOMNOCCC9990900\001"
        "20=3\001150=E\00139=E\00155=MSFT\001167=CS\00154=1\001"
        "38=15\00140=2\00144=15\00158=PHLX EQUITY TESTING\00159=0\001"
        "47=C\00132=0\00131=0\001151=15\00114=0\0016=0\001";

    EXPECT_EQ(128, fix::checksum(reinterpret_cast<const std::byte*>(message),
                                 sizeof(message) - 1));
}
