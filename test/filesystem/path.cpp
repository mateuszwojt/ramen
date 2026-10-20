// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<gtest/gtest.h>

#include<filesystem>

#include<ramen/filesystem/path.hpp>

using namespace ramen;

namespace
{

std::filesystem::path root() { return std::filesystem::current_path().root_path();}

} // unnamed

TEST( Path, MakeAbsolute)
{
    std::filesystem::path base( root() / "comps" / "shot");

    EXPECT_EQ( filesystem::make_absolute_path( "plates/a.exr", base), base / "plates" / "a.exr");
    EXPECT_EQ( filesystem::make_absolute_path( "../plates/./a.exr", base), root() / "comps" / "plates" / "a.exr");
}

TEST( Path, MakeRelative)
{
    std::filesystem::path base( root() / "comps" / "shot");

    EXPECT_EQ( filesystem::make_relative_path( base / "plates" / "a.exr", base), std::filesystem::path( "plates") / "a.exr");
    EXPECT_EQ( filesystem::make_relative_path( root() / "comps" / "plates" / "a.exr", base), std::filesystem::path( "..") / "plates" / "a.exr");
}

TEST( Path, ConvertRelative)
{
    std::filesystem::path old_base( root() / "comps" / "shot");
    std::filesystem::path new_base( root() / "comps" / "shot" / "v2");

    EXPECT_EQ( filesystem::convert_relative_path( std::filesystem::path( "plates") / "a.exr", old_base, new_base),
               std::filesystem::path( "..") / "plates" / "a.exr");
}

TEST( Path, Utf8)
{
    std::string name( "pl\xc3\xa1te.exr");
    EXPECT_EQ( filesystem::file_string( filesystem::path_from_utf8( name)), name);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest( &argc, argv);
    return RUN_ALL_TESTS();
}
