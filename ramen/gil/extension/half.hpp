//  Copyright Esteban Tovagliari 2008. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef GIL_EXTENSION_HALF_HPP
#define GIL_EXTENSION_HALF_HPP

#include<Imath/half.h>

#include<boost/gil/channel.hpp>
#include<boost/gil/channel_algorithm.hpp>
#include<boost/gil/typedefs.hpp>

namespace boost
{
namespace gil
{

// half is a class, but GIL has to treat it like a built-in numeric channel.
template<> struct channel_traits<half> : detail::channel_traits_impl<half, false> {};
template<> struct channel_traits<const half> : detail::channel_traits_impl<const half, false> {};

struct half_zero { static half apply() { return half( 0.0f);}};
struct half_one  { static half apply() { return half( 1.0f);}};

typedef scoped_channel_value<half,half_zero,half_one> bits16f;

// channel conversion
template <> struct channel_converter_unsigned<bits16f,uint8_t>
{
    uint8_t operator()(bits16f x) const { return static_cast<uint8_t>( half(x) * 255.0f + 0.5f);}
};

template <> struct channel_converter_unsigned<uint8_t,bits16f>
{
    bits16f operator()(uint8_t   x) const { return static_cast<bits16f>( half(x) / 255.0f);}
};

template <> struct channel_converter_unsigned<bits16f,uint16_t>
{
    uint16_t operator()(bits16f x) const { return static_cast<uint16_t>( half(x) * 65535.0f + 0.5f);}
};

template <> struct channel_converter_unsigned<uint16_t,bits16f>
{
    bits16f operator()(uint16_t  x) const { return static_cast<bits16f>( float(x) / 65535.0f);}
};

// floating point
template <> struct channel_converter_unsigned<float32_t,bits16f>
{
    bits16f operator()(float32_t  x) const { return bits16f( half(x));}
};

template <> struct channel_converter_unsigned<bits16f,float32_t>
{
    float32_t operator()(bits16f  x) const { return float32_t( half(x));}
};

template<> struct channel_multiplier_unsigned<bits16f>
{
    bits16f operator()(bits16f a, bits16f b) const { return bits16f( half(a) * half(b));}
};

// typedefs
BOOST_GIL_DEFINE_BASE_TYPEDEFS(16f, bits16f, bgr)
BOOST_GIL_DEFINE_BASE_TYPEDEFS(16f, bits16f, argb)
BOOST_GIL_DEFINE_BASE_TYPEDEFS(16f, bits16f, abgr)
BOOST_GIL_DEFINE_BASE_TYPEDEFS(16f, bits16f, bgra)

BOOST_GIL_DEFINE_ALL_TYPEDEFS(16f, bits16f, gray)
BOOST_GIL_DEFINE_ALL_TYPEDEFS(16f, bits16f, rgb)
BOOST_GIL_DEFINE_ALL_TYPEDEFS(16f, bits16f, rgba)
BOOST_GIL_DEFINE_ALL_TYPEDEFS(16f, bits16f, cmyk)

template <int N> struct devicen_t;
template <int N> struct devicen_layout_t;

BOOST_GIL_DEFINE_ALL_TYPEDEFS_INTERNAL(16f, bits16f, dev2n, devicen_t<2>, devicen_layout_t<2>)
BOOST_GIL_DEFINE_ALL_TYPEDEFS_INTERNAL(16f, bits16f, dev3n, devicen_t<3>, devicen_layout_t<3>)
BOOST_GIL_DEFINE_ALL_TYPEDEFS_INTERNAL(16f, bits16f, dev4n, devicen_t<4>, devicen_layout_t<4>)
BOOST_GIL_DEFINE_ALL_TYPEDEFS_INTERNAL(16f, bits16f, dev5n, devicen_t<5>, devicen_layout_t<5>)

} // namespace
} // namespace

#endif
