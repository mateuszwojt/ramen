// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<gtest/gtest.h>
#include<utility>

#include<ramen/core/memory.hpp>

#include<vector>

using namespace ramen::core;

void sink_function( auto_ptr_t<int>&& x)
{
    auto_ptr_t<int> z( std::move( x));
}

auto_ptr_t<int> source_function()
{
    return auto_ptr_t<int>( new int());
}

TEST( AutoPtr, Construct)
{
    auto_ptr_t<int> x;
    ASSERT_FALSE( x.get());

    auto_ptr_t<int> y( new int());
    ASSERT_TRUE( y);

    x.reset( y.release());
    ASSERT_TRUE( x);
    ASSERT_FALSE( y.get());
}

TEST( AutoPtr, Move)
{
    auto_ptr_t<int> x;
    EXPECT_FALSE( x.get());

    auto_ptr_t<int> y( new int());
    EXPECT_TRUE( y);

    x = std::move( y);
    EXPECT_TRUE( x);
    EXPECT_FALSE( y.get());

    auto_ptr_t<int> z( std::move( x));
    EXPECT_TRUE( z);
    EXPECT_FALSE( x.get());
}

TEST( AutoPtr, SinkSource)
{
    auto_ptr_t<int> y( new int());
    EXPECT_TRUE( y);

    sink_function( std::move( y));
    EXPECT_FALSE( y.get());

    auto_ptr_t<int> yy = source_function();
    EXPECT_TRUE( yy);
}

TEST( AutoPtr, Container)
{
    std::vector<auto_ptr_t<int> > vec;
    auto_ptr_t<int> y( new int( 11));

    vec.push_back( std::move( y));
    EXPECT_FALSE( y.get());
    EXPECT_EQ( vec.size(), 1);

    auto_ptr_t<int> z( new int( 1));
    vec.push_back( std::move( z));
    EXPECT_FALSE( z.get());
    EXPECT_EQ( vec.size(), 2);

    EXPECT_EQ( *vec[0], 11);
    EXPECT_EQ( *vec[1], 1);
}

TEST( AlignedStorage, All)
{
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest( &argc, argv);
    return RUN_ALL_TESTS();
}
