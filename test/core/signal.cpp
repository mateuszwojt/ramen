// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<gtest/gtest.h>

#include<ramen/core/signal.hpp>

#include<vector>

using namespace ramen::core;

TEST( Signal, ConnectCallDisconnect)
{
    signal_t<void ( int)> sig;
    EXPECT_TRUE( sig.empty());

    std::vector<int> calls;
    connection_t a = sig.connect( [&calls]( int x) { calls.push_back( x);});
    connection_t b = sig.connect( [&calls]( int x) { calls.push_back( x * 10);});
    EXPECT_FALSE( sig.empty());
    EXPECT_TRUE( a.connected());

    sig( 2);
    ASSERT_EQ( calls.size(), 2);
    EXPECT_EQ( calls[0], 2);
    EXPECT_EQ( calls[1], 20);

    a.disconnect();
    EXPECT_FALSE( a.connected());

    sig( 3);
    ASSERT_EQ( calls.size(), 3);
    EXPECT_EQ( calls[2], 30);

    sig.disconnect_all_slots();
    EXPECT_FALSE( b.connected());
    EXPECT_TRUE( sig.empty());
}

TEST( Signal, DisconnectWhileCalling)
{
    signal_t<void ()> sig;
    connection_t c;
    int n = 0;
    c = sig.connect( [&]() { ++n; c.disconnect();});

    sig();
    sig();
    EXPECT_EQ( n, 1);
}

TEST( Signal, ConnectionOutlivesSignal)
{
    connection_t c;
    {
        signal_t<void ()> sig;
        c = sig.connect( []() {});
        EXPECT_TRUE( c.connected());
    }

    EXPECT_FALSE( c.connected());
    c.disconnect();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest( &argc, argv);
    return RUN_ALL_TESTS();
}
