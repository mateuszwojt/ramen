// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_CONTAINER_PTR_VECTOR_HPP
#define RAMEN_CONTAINER_PTR_VECTOR_HPP

#include<cstddef>
#include<deque>
#include<iterator>
#include<memory>
#include<type_traits>
#include<utility>
#include<vector>

#include<ramen/assert.hpp>

namespace ramen
{
namespace container
{

/*!
\ingroup container
\brief Iterator adaptor that dereferences the unique_ptrs of an owning sequence.
*/
template<class BaseIter, class T>
class indirect_iterator
{
public:

    typedef std::random_access_iterator_tag iterator_category;
    typedef std::random_access_iterator_tag iterator_concept;
    typedef std::remove_cv_t<T>             value_type;
    typedef std::ptrdiff_t                  difference_type;
    typedef T*                              pointer;
    typedef T&                              reference;

    indirect_iterator() = default;
    explicit indirect_iterator( BaseIter it) : it_( it) {}

    // iterator to const_iterator conversion.
    template<class OtherIter, class U, class = std::enable_if_t<std::is_convertible_v<OtherIter, BaseIter> > >
    indirect_iterator( const indirect_iterator<OtherIter, U>& other) : it_( other.base()) {}

    const BaseIter& base() const { return it_;}

    reference operator*() const     { return **it_;}
    pointer operator->() const      { return it_->get();}
    reference operator[]( difference_type n) const { return *it_[n];}

    indirect_iterator& operator++()     { ++it_; return *this;}
    indirect_iterator operator++( int)  { indirect_iterator tmp( *this); ++it_; return tmp;}
    indirect_iterator& operator--()     { --it_; return *this;}
    indirect_iterator operator--( int)  { indirect_iterator tmp( *this); --it_; return tmp;}

    indirect_iterator& operator+=( difference_type n) { it_ += n; return *this;}
    indirect_iterator& operator-=( difference_type n) { it_ -= n; return *this;}

    friend indirect_iterator operator+( indirect_iterator it, difference_type n) { return it += n;}
    friend indirect_iterator operator+( difference_type n, indirect_iterator it) { return it += n;}
    friend indirect_iterator operator-( indirect_iterator it, difference_type n) { return it -= n;}
    friend difference_type operator-( const indirect_iterator& a, const indirect_iterator& b) { return a.it_ - b.it_;}

    friend bool operator==( const indirect_iterator& a, const indirect_iterator& b) { return a.it_ == b.it_;}
    friend auto operator<=>( const indirect_iterator& a, const indirect_iterator& b) { return a.it_ <=> b.it_;}

private:

    BaseIter it_;
};

/*!
\ingroup container
\brief Sequence that owns heap allocated, possibly polymorphic, objects.
Iteration yields references to the objects. Copies clone the elements using new_clone.
*/
template<class T, class Seq>
class basic_ptr_sequence
{
public:

    typedef T                           value_type;
    typedef T&                          reference;
    typedef const T&                    const_reference;
    typedef std::size_t                 size_type;
    typedef std::unique_ptr<T>          auto_type;

    typedef indirect_iterator<typename Seq::iterator, T>                    iterator;
    typedef indirect_iterator<typename Seq::const_iterator, const T>        const_iterator;
    typedef std::reverse_iterator<iterator>                                 reverse_iterator;
    typedef std::reverse_iterator<const_iterator>                           const_reverse_iterator;

    basic_ptr_sequence() = default;

    basic_ptr_sequence( const basic_ptr_sequence& other)
    {
        for( const T& x : other)
            push_back( new_clone( x));
    }

    basic_ptr_sequence( basic_ptr_sequence&& other) = default;

    basic_ptr_sequence& operator=( const basic_ptr_sequence& other)
    {
        basic_ptr_sequence tmp( other);
        swap( tmp);
        return *this;
    }

    basic_ptr_sequence& operator=( basic_ptr_sequence&& other) = default;

    // iterators
    iterator begin()                { return iterator( seq_.begin());}
    iterator end()                  { return iterator( seq_.end());}
    const_iterator begin() const    { return const_iterator( seq_.begin());}
    const_iterator end() const      { return const_iterator( seq_.end());}

    reverse_iterator rbegin()               { return reverse_iterator( end());}
    reverse_iterator rend()                 { return reverse_iterator( begin());}
    const_reverse_iterator rbegin() const   { return const_reverse_iterator( end());}
    const_reverse_iterator rend() const     { return const_reverse_iterator( begin());}

    // size
    bool empty() const      { return seq_.empty();}
    size_type size() const  { return seq_.size();}

    // element access
    reference operator[]( size_type i)              { return *seq_[i];}
    const_reference operator[]( size_type i) const  { return *seq_[i];}

    reference front()               { return *seq_.front();}
    const_reference front() const   { return *seq_.front();}
    reference back()                { return *seq_.back();}
    const_reference back() const    { return *seq_.back();}

    // modifiers
    void push_back( T *x)
    {
        RAMEN_ASSERT( x && "Null pointer added to ptr container");
        seq_.emplace_back( x);
    }

    template<class U>
    void push_back( std::unique_ptr<U> x)
    {
        RAMEN_ASSERT( x && "Null pointer added to ptr container");
        seq_.emplace_back( std::move( x));
    }

    iterator insert( iterator pos, T *x)
    {
        RAMEN_ASSERT( x && "Null pointer added to ptr container");
        return iterator( seq_.emplace( pos.base(), x));
    }

    template<class U>
    iterator insert( iterator pos, std::unique_ptr<U> x)
    {
        RAMEN_ASSERT( x && "Null pointer added to ptr container");
        return iterator( seq_.emplace( pos.base(), std::move( x)));
    }

    iterator erase( iterator pos)                   { return iterator( seq_.erase( pos.base()));}
    iterator erase( iterator first, iterator last)  { return iterator( seq_.erase( first.base(), last.base()));}

    void clear() { seq_.clear();}

    /// Removes the element from the container and returns ownership of it.
    auto_type release( iterator pos)
    {
        auto_type result( std::move( *pos.base()));
        seq_.erase( pos.base());
        return result;
    }

    auto_type pop_back()
    {
        auto_type result( std::move( seq_.back()));
        seq_.pop_back();
        return result;
    }

    auto_type pop_front()
    {
        auto_type result( std::move( seq_.front()));
        seq_.erase( seq_.begin());
        return result;
    }

    void swap( basic_ptr_sequence& other) { seq_.swap( other.seq_);}

private:

    Seq seq_;
};

template<class T>
using ptr_vector = basic_ptr_sequence<T, std::vector<std::unique_ptr<T> > >;

template<class T>
using ptr_deque = basic_ptr_sequence<T, std::deque<std::unique_ptr<T> > >;

} // container
} // ramen

#endif
