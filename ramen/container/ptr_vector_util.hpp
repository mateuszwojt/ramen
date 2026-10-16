// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_CONTAINER_PTR_VECTOR_UTIL_HPP
#define RAMEN_CONTAINER_PTR_VECTOR_UTIL_HPP

#include<memory>
#include<algorithm>

#include<ramen/container/ptr_vector.hpp>

#include<ramen/assert.hpp>

namespace ramen
{
namespace container
{

template<class T>
std::unique_ptr<T> release_ptr( T *p, container::ptr_vector<T>& vec)
{
    std::unique_ptr<T> result;

	for( typename container::ptr_vector<T>::iterator it( vec.begin()), e( vec.end()); it != e; ++it)
	{
		if( &(*it) == p)
		{
			typename container::ptr_vector<T>::auto_type ptr = vec.release( it);
            result.reset( ptr.release());
            break;
		}
	}

	return result;
}

template<class Iter, class T>
void swap_elements( Iter first, Iter second, container::ptr_vector<T>& vec)
{
    std::swap( *first.base(), *second.base());
}

} // container
} // ramen

#endif
