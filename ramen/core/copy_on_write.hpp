// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_CORE_COPY_ON_WRITE_HPP
#define RAMEN_CORE_COPY_ON_WRITE_HPP

#include<ramen/config.hpp>
#include<concepts>
#include<type_traits>

#include<memory>
#include<utility>

#include<ramen/Concepts/RegularConcept.hpp>

#include<ramen/assert.hpp>

namespace ramen
{
namespace core
{

/*!
\ingroup core
\brief Copy on write holder.
*/
template <typename T> // T models Regular
class copy_on_write_t
{
    static_assert( RegularConcept<T>);

public:

    typedef T value_type;

    copy_on_write_t()
    {
        object_.reset( new implementation_t(), capture_deleter_t());
    }

    explicit copy_on_write_t( const T& x)
    {
        object_.reset( new implementation_t( x), capture_deleter_t());
    }

    explicit copy_on_write_t( T&& x)
    {
        object_.reset( new implementation_t( std::move( x)), capture_deleter_t());
    }

    const value_type& read() const
    {
        RAMEN_ASSERT( object_);

        return object_->value_;
    }

    operator const value_type& () const     { return read();}
    const value_type& operator*() const     { return read();}
    const value_type* operator->() const    { return &read();}

    value_type& write()
    {
        RAMEN_ASSERT( object_);

        if( !unique_instance())
            object_.reset( new implementation_t( object_->value_), capture_deleter_t());

        return object_->value_;
    }

    bool unique_instance() const
    {
        return object_ && object_.use_count() == 1;
    }

    void swap( copy_on_write_t& other)
    {
        std::swap( object_, other.object_);
    }

private:

    struct implementation_t
    {
        implementation_t() {}

        explicit implementation_t( const value_type& x) : value_( x) {}

        explicit implementation_t( value_type&& x) : value_( std::move( x)) {}

        value_type value_;
    };

    struct capture_deleter_t
    {
        void operator()( implementation_t *x) const { delete x;}
    };

    typedef std::shared_ptr<implementation_t> implementation_ptr_t;

    implementation_ptr_t object_;
};

template<class T>
inline void swap( copy_on_write_t<T>& x, copy_on_write_t<T>& y)
{
    x.swap( y);
}

template<class T>
std::enable_if_t<std::equality_comparable<T>, bool>
operator==( const copy_on_write_t<T>& a, const copy_on_write_t<T>& b)
{
    return a.read() == b.read();
}

template<class T>
std::enable_if_t<std::equality_comparable<T>, bool>
operator!=( const copy_on_write_t<T>& a, const copy_on_write_t<T>& b)
{
    return a.read() != b.read();
}

} // core
} // ramen

#endif
