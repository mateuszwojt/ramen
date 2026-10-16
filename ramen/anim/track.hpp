// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_ANIM_TRACK_HPP
#define	RAMEN_ANIM_TRACK_HPP

#include<string>
#include<memory>

#include<optional>
#include<ramen/container/ptr_vector.hpp>
#include<ramen/core/signal.hpp>

#include<Imath/ImathColor.h>

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/anim/any_curve.hpp>

namespace ramen
{
namespace anim
{

class track_t
{
public:

    track_t();
    explicit track_t( const std::string& name);
    track_t( const std::string& name, const any_curve_ptr_t& curve);

    track_t *parent() const         { return parent_;}
    void set_parent( track_t *p)    { parent_ = p;}

    int num_children() const { return (int) children_.size();}

    track_t *child( int i);

    void add_child( std::unique_ptr<track_t> t);

    void clear_children();

    int row() const;

    // ranges
    const container::ptr_vector<track_t>& children() const	{ return children_;}
    container::ptr_vector<track_t>& children()              { return children_;}

    const std::string& name() const;
    void set_name( const std::string& name);

	const std::string& full_name() const;
	void make_full_names();

	const std::string& curve_name() const;
	
    bool is_leaf() const { return num_children() == 0;}

    const Imath::Color3c& color() const { return color_;}
    void set_color( const Imath::Color3c& col);

    // observer
    core::signal_t<void ( any_curve_ptr_t&)> changed;

    void notify();

    const std::optional<any_curve_ptr_t>& curve() const   { return curve_;}
    std::optional<any_curve_ptr_t>& curve()               { return curve_;}

private:

	void init();

	void do_make_full_names();

	void make_curve_name();
	
    std::string name_, full_name_, curve_name_;
    track_t *parent_;
    container::ptr_vector<track_t> children_;
    std::optional<any_curve_ptr_t> curve_;
    Imath::Color3c color_;
};

template<class Fun>
void for_each_track_depth_first( track_t *t, Fun f)
{
    f( t);

    for( int i = 0; i < t->num_children(); ++i)
        for_each_track_depth_first( t->child( i), f);
}

template<class Fun>
void for_each_leaf_track( track_t *t, Fun f)
{
    if( t->is_leaf())
        f( t);
    else
    {
        for( int i = 0; i < t->num_children(); ++i)
            for_each_leaf_track( t->child( i), f);
    }
}

template<class Fun>
void for_each_curve( track_t *t, Fun f)
{
    if( t->is_leaf() && t->curve())
        f( t->curve().value());
    else
    {
        for( int i = 0; i < t->num_children(); ++i)
            for_each_curve( t->child( i), f);
    }
}

} // namespace
} // namespace

#endif
