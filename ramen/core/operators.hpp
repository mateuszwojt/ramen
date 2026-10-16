// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_CORE_OPERATORS_HPP
#define RAMEN_CORE_OPERATORS_HPP

// Base classes that define binary arithmetic operators in terms of their
// compound assignment versions. Comparison operators are not needed in C++20:
// operator!= is rewritten in terms of operator==.

namespace ramen
{
namespace core
{

template<class T>
struct addable
{
    friend T operator+( T x, const T& y) { x += y; return x;}
};

template<class T>
struct subtractable
{
    friend T operator-( T x, const T& y) { x -= y; return x;}
};

template<class T>
struct multipliable
{
    friend T operator*( T x, const T& y) { x *= y; return x;}
};

template<class T, class U>
struct addable2
{
    friend T operator+( T x, const U& y) { x += y; return x;}
    friend T operator+( const U& y, T x) { x += y; return x;}
};

template<class T, class U>
struct subtractable2
{
    friend T operator-( T x, const U& y) { x -= y; return x;}
};

template<class T, class U>
struct multipliable2
{
    friend T operator*( T x, const U& y) { x *= y; return x;}
    friend T operator*( const U& y, T x) { x *= y; return x;}
};

template<class T, class U>
struct dividable2
{
    friend T operator/( T x, const U& y) { x /= y; return x;}
};

} // core
} // ramen

#endif
