// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_CONFIG_COMPILER_HPP
#define RAMEN_CONFIG_COMPILER_HPP

#if defined( __GNUC__)
    #define RAMEN_CONFIG_COMPILER_GNUC
#elif defined( __clang__)
    #define RAMEN_CONFIG_COMPILER_CLANG
#elif defined( _MSC_VER)
    #define RAMEN_CONFIG_COMPILER_MSVC
#elif defined( __CYGWIN__)
    #define RAMEN_CONFIG_COMPILER_CYGWIN
#endif

#if defined( RAMEN_CONFIG_COMPILER_MSVC)
    #define RAMEN_FORCEINLINE __forceinline
#elif defined( __GNUC__) || defined( __clang__)
    #define RAMEN_FORCEINLINE inline __attribute__(( always_inline))
#else
    #define RAMEN_FORCEINLINE inline
#endif

// MSVC only applies the empty base optimization to the first base class unless asked to.
#if defined( RAMEN_CONFIG_COMPILER_MSVC)
    #define RAMEN_EMPTY_BASES __declspec( empty_bases)
#else
    #define RAMEN_EMPTY_BASES
#endif

#endif
