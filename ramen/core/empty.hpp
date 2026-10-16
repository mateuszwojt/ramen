// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_CORE_EMPTY_HPP
#define RAMEN_CORE_EMPTY_HPP

#include<ramen/config.hpp>

#include<compare>

namespace ramen
{
namespace core
{

/*!
\ingroup core
\brief Empty class.
*/
struct RAMEN_API empty_t
{
    bool operator==( const empty_t&) const;
    bool operator<( const empty_t&) const;

    std::strong_ordering operator<=>( const empty_t&) const { return std::strong_ordering::equal;}

    void swap( empty_t&);
};

inline void swap( empty_t&, empty_t&) {}

} // core
} // ramen

#endif
