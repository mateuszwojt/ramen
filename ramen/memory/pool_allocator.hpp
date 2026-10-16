// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_MEMORY_POOL_ALLOCATOR_HPP
#define RAMEN_MEMORY_POOL_ALLOCATOR_HPP

#include<ramen/config.hpp>

#include<vector>
#include<memory>

#include<ramen/core/noncopyable.hpp>
#include<cstdint>
#include<mutex>


#include<ramen/assert.hpp>

#include<ramen/memory/lru_cache_interface.hpp>

namespace ramen
{
namespace memory
{

template<class MemPool>
class pool_allocator_t : ramen::core::noncopyable_t
{
public:

    explicit pool_allocator_t( std::uint64_t size)
    {
        pool_.init( size);
    }

	void add_cache( lru_cache_interface *c) { caches_.push_back( c);}

    std::uint64_t max_size() const { return pool_.pool_size();}

    unsigned char *allocate( std::size_t& s)
	{
		if( s == 0)
			return 0;

	    std::lock_guard<std::recursive_mutex> lock( mutex_);

		while( 1)
	    {
		    unsigned char *ptr = pool_.allocate( s);

			if( ptr)
				return ptr;

			if( !erase_lru())
				break;
		}

        // if we get here, it means there's not enough memory & all caches are empty.
	    throw std::bad_alloc();
		s = 0;
		return 0;
	}

    void deallocate( unsigned char *p, std::size_t s)
	{
		if( p == 0)
			return;

		RAMEN_ASSERT( s != 0);

	    std::lock_guard<std::recursive_mutex> lock( mutex_);
	    pool_.deallocate( p, s);
	}

private:

	bool erase_lru()
	{
		int lru_cache = -1;
		std::chrono::steady_clock::time_point oldest = std::chrono::steady_clock::now();

		for( int i = 0, ie = caches_.size(); i < ie; ++i)
		{
			if( !caches_[i]->empty())
			{
				std::chrono::steady_clock::time_point touch_time = caches_[i]->lru_time();

				if( touch_time < oldest)
				{
					lru_cache = i;
					oldest = touch_time;
				}
			}
		}

		if( lru_cache < 0)
			return false;

		caches_[lru_cache]->erase_lru();
		return true;
	}

    std::recursive_mutex mutex_;
    MemPool pool_;

	std::vector<lru_cache_interface*> caches_;
};

} // namespace
} // namespace

#endif
