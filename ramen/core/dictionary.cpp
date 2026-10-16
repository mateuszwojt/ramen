// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/core/dictionary.hpp>

#include<algorithm>

#include<utility>
#include<vector>

#include<ramen/core/exceptions.hpp>

namespace ramen
{
namespace core
{

struct dictionary_t::impl
{
    typedef std::pair<key_type, value_type> item_type;

    // a map stored as a sorted vector, iterators are pointers to the items.
    struct map_type : public std::vector<item_type>
    {
        typedef std::vector<item_type> base_type;

        typename base_type::iterator lower_bound( const key_type& key)
        {
            return std::lower_bound( begin(), end(), key, compare);
        }

        typename base_type::const_iterator lower_bound( const key_type& key) const
        {
            return std::lower_bound( begin(), end(), key, compare);
        }

        typename base_type::iterator find( const key_type& key)
        {
            typename base_type::iterator it( lower_bound( key));
            return ( it != end() && !( key < it->first)) ? it : end();
        }

        typename base_type::const_iterator find( const key_type& key) const
        {
            typename base_type::const_iterator it( lower_bound( key));
            return ( it != end() && !( key < it->first)) ? it : end();
        }

        std::pair<typename base_type::iterator, bool> insert( const item_type& x)
        {
            typename base_type::iterator it( lower_bound( x.first));

            if( it != end() && !( x.first < it->first))
                return std::make_pair( it, false);

            return std::make_pair( base_type::insert( it, x), true);
        }

    private:

        static bool compare( const item_type& x, const key_type& key) { return x.first < key;}
    };

    typedef map_type::const_iterator    const_iterator;
    typedef map_type::iterator          iterator;

    map_type items;
};

dictionary_t::dictionary_t() : pimpl_( 0) {}

dictionary_t::~dictionary_t()
{
    delete pimpl_;
}

// Copy constructor
dictionary_t::dictionary_t( const dictionary_t& other) : pimpl_( 0)
{
    if( other.pimpl_)
        pimpl_ = new impl( *other.pimpl_);
}

void dictionary_t::swap( dictionary_t& other)
{
    std::swap( pimpl_, other.pimpl_);
}

bool dictionary_t::empty() const
{
    if( pimpl_)
        return pimpl_->items.empty();

    return true;
}

dictionary_t::size_type dictionary_t::size() const
{
    if( pimpl_)
        return pimpl_->items.size();

    return 0;
}

void dictionary_t::clear()
{
    if( pimpl_)
        pimpl_->items.clear();
}

const dictionary_t::value_type& dictionary_t::operator[]( const dictionary_t::key_type& key) const
{
    if( !pimpl_)
        throw key_not_found( key);

    impl::const_iterator it( pimpl_->items.find( key ));

    if( it != pimpl_->items.end())
        return it->second;
    else
        throw key_not_found( key);
}

dictionary_t::value_type& dictionary_t::operator[]( const dictionary_t::key_type& key)
{
    if( !pimpl_)
        pimpl_ = new impl();

    impl::iterator it( pimpl_->items.find( key ));

    if( it != pimpl_->items.end())
        return it->second;
    else
    {
        std::pair<impl::iterator, bool> new_it( pimpl_->items.insert( std::make_pair( key, dictionary_t::value_type())));
        return new_it.first->second;
    }
}

dictionary_t::const_iterator dictionary_t::begin() const
{
    if( pimpl_)
        return reinterpret_cast<const_iterator>( pimpl_->items.data());

    return 0;
}

dictionary_t::const_iterator dictionary_t::end() const
{
    if( pimpl_)
        return reinterpret_cast<const_iterator>( ( pimpl_->items.data() + pimpl_->items.size()));

    return 0;
}

dictionary_t::iterator dictionary_t::begin()
{
    if( pimpl_)
        return reinterpret_cast<iterator>( pimpl_->items.data());

    return 0;
}

dictionary_t::iterator dictionary_t::end()
{
    if( pimpl_)
        return reinterpret_cast<iterator>( ( pimpl_->items.data() + pimpl_->items.size()));

    return 0;
}

bool dictionary_t::operator==( const dictionary_t& other) const
{
    if( pimpl_ == 0 && other.pimpl_ == 0)
        return true;

    if( pimpl_ != 0 && other.pimpl_ != 0)
        return pimpl_->items == other.pimpl_->items;

    return false;
}

bool dictionary_t::operator!=( const dictionary_t& other) const
{
    return !( *this == other);
}

const dictionary_t::value_type *dictionary_t::get( const dictionary_t::key_type& key) const
{
    if( !pimpl_)
        return 0;

    impl::const_iterator it( pimpl_->items.find( key ));

    if( it != pimpl_->items.end())
        return &( it->second);

    return 0;
}

dictionary_t::value_type *dictionary_t::get( const dictionary_t::key_type& key)
{
    if( !pimpl_)
        return 0;

    impl::iterator it( pimpl_->items.find( key ));

    if( it != pimpl_->items.end())
        return &( it->second);

    return 0;
}

} // core
} // ramen
