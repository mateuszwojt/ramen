// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<gtest/gtest.h>

#include<algorithm>
#include<memory>

#include<ramen/container/ptr_vector.hpp>

using namespace ramen::container;

namespace
{

struct base_t
{
    explicit base_t( int v) : value( v) {}
    virtual ~base_t() {}
    virtual base_t *clone() const { return new base_t( *this);}

    int value;
};

struct derived_t : public base_t
{
    explicit derived_t( int v) : base_t( v) {}
    base_t *clone() const override { return new derived_t( *this);}
};

base_t *new_clone( const base_t& x) { return x.clone();}

} // unnamed

TEST( PtrVector, Basic)
{
    ptr_vector<base_t> v;
    EXPECT_TRUE( v.empty());

    v.push_back( new base_t( 1));
    v.push_back( std::unique_ptr<derived_t>( new derived_t( 2)));
    v.insert( v.begin(), new base_t( 0));

    ASSERT_EQ( v.size(), 3);
    EXPECT_EQ( v[0].value, 0);
    EXPECT_EQ( v.back().value, 2);
    EXPECT_TRUE( dynamic_cast<derived_t*>( &v[2]));

    int sum = 0;
    for( const base_t& x : v)
        sum += x.value;

    EXPECT_EQ( sum, 3);
    EXPECT_EQ( std::ranges::count_if( v, []( const base_t& x) { return x.value > 0;}), 2);
    EXPECT_EQ( v.rbegin()->value, 2);
}

TEST( PtrVector, ReleaseAndPop)
{
    ptr_vector<base_t> v;
    v.push_back( new base_t( 1));
    v.push_back( new base_t( 2));
    v.push_back( new base_t( 3));

    std::unique_ptr<base_t> p( v.release( v.begin() + 1));
    EXPECT_EQ( p->value, 2);
    ASSERT_EQ( v.size(), 2);
    EXPECT_EQ( v[1].value, 3);

    std::unique_ptr<base_t> q( v.pop_back());
    EXPECT_EQ( q->value, 3);
    ASSERT_EQ( v.size(), 1);

    v.erase( v.begin());
    EXPECT_TRUE( v.empty());
}

TEST( PtrVector, CopyClones)
{
    ptr_vector<base_t> v;
    v.push_back( new derived_t( 7));

    ptr_vector<base_t> w( v);
    ASSERT_EQ( w.size(), 1);
    EXPECT_NE( &w[0], &v[0]);
    EXPECT_EQ( w[0].value, 7);
    EXPECT_TRUE( dynamic_cast<derived_t*>( &w[0]));
}

TEST( PtrDeque, PopFront)
{
    ptr_deque<base_t> d;
    d.push_back( new base_t( 1));
    d.push_back( new base_t( 2));

    std::unique_ptr<base_t> p( d.pop_front());
    EXPECT_EQ( p->value, 1);
    EXPECT_EQ( d.front().value, 2);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest( &argc, argv);
    return RUN_ALL_TESTS();
}
