// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<gtest/gtest.h>
#include<utility>

#include<cstring>
#include<iostream>

#include<ramen/core/string.hpp>

#include<algorithm>
#include<algorithm>
#include<iterator>
#include<ranges>
#include<string>
#include<vector>
#include<ramen/util/lexical_cast.hpp>

using namespace ramen::core;

TEST( String8, Construct)
{
    string_t a( "XYZ");
    ASSERT_EQ( a.size(), 3);
    ASSERT_EQ( strcmp( a.c_str(), "XYZ"), 0);

    string_t b( "The quick brown fox jumps over the lazy dog");
    string_t part0( b, 0, 9);
    ASSERT_EQ( part0, "The quick");

    string_t part1( b, 10, 5);
    ASSERT_EQ( part1, "brown");

    string_t part2( b, b.length() - 3, 150);
    ASSERT_EQ( part2, "dog");
}

TEST( String8, CopyAssign)
{
    string_t a( "XYZ");
    string_t b( a);

    ASSERT_EQ( a.size(), b.size());
    ASSERT_EQ( strcmp( a.c_str(), b.c_str()), 0);
    ASSERT_EQ( strcmp( a.c_str(), "XYZ"), 0);
    ASSERT_NE( a.c_str(), b.c_str());
}

TEST( String8, MoveAssign)
{
    string_t a( "XYZ");
    string_t b( "XYZ");

    string_t c( std::move( b));
    EXPECT_EQ( a.size(), 3);
    EXPECT_EQ( c, a);

    b = std::move( c);
    EXPECT_EQ( b.size(), 3);
    EXPECT_EQ( b, a);
}

TEST( String8, Swap)
{
    string_t sp( "Structured procrastination");
    string_t wa( "Wasabi alarm");

    sp.swap( wa);

    EXPECT_EQ( strcmp( sp.c_str(), "Wasabi alarm"), 0);
    EXPECT_EQ( strcmp( wa.c_str(), "Structured procrastination"), 0);
}

TEST( String8, Compare)
{
    string_t sp( "Structured procrastination");
    string_t sp2( "Structured procrastination");

    string_t wa( "Wasabi alarm");
    string_t wa2( "Wasabi alarm");

    EXPECT_EQ( strcmp( wa.c_str(), "Wasabi alarm"), 0);
    EXPECT_EQ( wa, wa2);
    EXPECT_NE( wa, sp2);

    EXPECT_EQ( strcmp( sp.c_str(), "Structured procrastination"), 0);
    EXPECT_EQ( sp, sp2);
    EXPECT_NE( sp, wa);
}

TEST( String8, Concat)
{
    string_t a( "XXX");
    a += "YYY";
    a += string_t( "ZZZ");

    EXPECT_EQ( a.size(), 9);
    EXPECT_EQ( strcmp( a.c_str(), "XXXYYYZZZ"), 0);
}

TEST( String8, Iterators)
{
    string_t a( "XYZ");
    string_t::const_iterator it( a.begin());
    string_t::const_iterator e( a.end());

    EXPECT_NE( it, e);
    EXPECT_EQ( std::distance( it, e), a.size());

    EXPECT_EQ( it[0], 'X');
    EXPECT_EQ( it[1], 'Y');
    EXPECT_EQ( it[2], 'Z');

    string_t b;
    EXPECT_EQ( b.begin(), b.end());
}

TEST( String8, MakeString)
{
    string_t str( make_string( "Xyzw ", "Yzw ", "Zw ", "W"));
    EXPECT_EQ( str, "Xyzw Yzw Zw W");

    str = make_string( "1234 ", "567 ", "89 ");
    EXPECT_EQ( str, "1234 567 89 ");

    str = make_string( string_t("1234").c_str(), "567");
    EXPECT_EQ( str, "1234567");
}

TEST( String8, OperatorPlus)
{
    string_t a( "XYZ");
    a = a + string_t( "ABC");
    a = a + "DEF";

    EXPECT_EQ( a.size(), 9);
    EXPECT_EQ( strcmp( a.c_str(), "XYZABCDEF"), 0);
}

struct nop_fun
{
    template<class T>
    void operator()( T x)
    {
    }
};

TEST( String8, BoostRange)
{
    string_t a( "XYZ");
    std::ranges::for_each( a, nop_fun());
}

TEST( String8, StdAlgorithms)
{
    string_t str( make_string( "Xyzw ", "Yzw ", "Zw ", "W"));
    std::reverse_iterator<string_t::iterator> rit( std::find( std::make_reverse_iterator( str.end()),
                                                              std::make_reverse_iterator( str.begin()), 'Z'));
    string_t::iterator it( rit.base() - 1);
    EXPECT_EQ( *it, 'Z');
    EXPECT_EQ( std::distance( str.begin(), it), 9);
}

TEST( String8, STLString)
{
    std::string stest( "My quick quasi constant 7312.");
    string_t dtest( stest);
    EXPECT_EQ( stest.size(), dtest.size());
    EXPECT_EQ( strcmp( stest.c_str(), dtest.c_str()), 0);

    std::string xtest( dtest.to_std_string());
    EXPECT_EQ( stest, xtest);
}

TEST( String8, Split)
{
    string_t test_path( "/usr/local/bin/any_bin");

    std::vector<std::string> tokens;
    for( auto token : std::views::split( test_path, '/'))
    {
        if( !std::ranges::empty( token))
            tokens.emplace_back( std::ranges::begin( token), std::ranges::end( token));
    }

    ASSERT_EQ( tokens.size(), 4);
    EXPECT_EQ( tokens[0], "usr");
    EXPECT_EQ( tokens[1], "local");
    EXPECT_EQ( tokens[2], "bin");
    EXPECT_EQ( tokens[3], "any_bin");
}

TEST( String8, LexicalCast)
{
    string_t str( "751");
    int x = ramen::util::lexical_cast<int>( str);
    EXPECT_EQ( x, 751);

    string_t str2 = ramen::util::lexical_cast<string_t>( x);
    EXPECT_EQ( str2, str);

    string_t str3( "any_string_123");
    EXPECT_ANY_THROW( ramen::util::lexical_cast<int>( str3));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest( &argc, argv);
    return RUN_ALL_TESTS();
}
