// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<gtest/gtest.h>

#include<ramen/util/calculator.hpp>

using namespace ramen::util;

TEST( Calculator, Expressions)
{
    calculator_t calc;

    EXPECT_DOUBLE_EQ( calc( "7").value(), 7.0);
    EXPECT_DOUBLE_EQ( calc( " 1 + 2 * 3 ").value(), 7.0);
    EXPECT_DOUBLE_EQ( calc( "(1 + 2) * 3").value(), 9.0);
    EXPECT_DOUBLE_EQ( calc( "10 - 4 - 3").value(), 3.0);
    EXPECT_DOUBLE_EQ( calc( "8 / 4 / 2").value(), 1.0);
    EXPECT_DOUBLE_EQ( calc( "-2 * -3").value(), 6.0);
    EXPECT_DOUBLE_EQ( calc( "+.5 + 1.25e1").value(), 13.0);
    EXPECT_DOUBLE_EQ( calc( "-(2 + 3)").value(), -5.0);
}

TEST( Calculator, Errors)
{
    calculator_t calc;

    EXPECT_FALSE( calc( ""));
    EXPECT_FALSE( calc( "1 +"));
    EXPECT_FALSE( calc( "(1 + 2"));
    EXPECT_FALSE( calc( "1 2"));
    EXPECT_FALSE( calc( "abc"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest( &argc, argv);
    return RUN_ALL_TESTS();
}
