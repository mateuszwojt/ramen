// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_CORE_NONCOPYABLE_HPP
#define RAMEN_CORE_NONCOPYABLE_HPP

namespace ramen
{
namespace core
{

/// Base class for classes that can't be copied.
class noncopyable_t
{
protected:

    noncopyable_t() = default;
    ~noncopyable_t() = default;

    noncopyable_t( const noncopyable_t&) = delete;
    noncopyable_t& operator=( const noncopyable_t&) = delete;
};

} // core
} // ramen

#endif
