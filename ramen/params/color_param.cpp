// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/color_param.hpp>

#include<limits>
#include<cmath>

#include<functional>


#include<ramen/app/application.hpp>

#include<ramen/assert.hpp>
#include<ramen/algorithm/clamp.hpp>

#include<ramen/nodes/node.hpp>
#include<ramen/app/document.hpp>

#include<ramen/anim/track.hpp>

namespace ramen
{

color_param_t::color_param_t( const std::string& name) : animated_param_t( name) { private_init();}

color_param_t::color_param_t( const color_param_t& other) : animated_param_t( other)
{
    is_rgba_ = other.is_rgba_;
}

void color_param_t::private_init()
{
    set_is_rgba( true);

    add_curve( "R");
    add_curve( "G");
    add_curve( "B");
    add_curve( "A");

    set_min( 0);
    curve( 3).set_range( 0, 1); // <- Alpha channel

    set_default_value( Imath::Color4f( 0, 0, 0, 0));
    set_step( 0.025f);
}

void color_param_t::set_default_value( const Imath::Color4f& x)
{
    poly_param_indexable_value_t v( x);
    value() = core::poly_cast<poly_param_value_t&>( v);
}

poly_param_value_t color_param_t::value_at_frame(float frame) const
{
    Imath::Color4f v( get_value<Imath::Color4f>( *this));
    eval_curve( 0, frame, v.r);
    eval_curve( 1, frame, v.g);
    eval_curve( 2, frame, v.b);

    if( is_rgba())
    {
        eval_curve( 3, frame, v.a);
        v.a = ramen::clamp( v.a, 0.0f, 1.0f);
    }
    else
        v.a = 1.0f;

    poly_param_indexable_value_t val( v);
    return core::poly_cast<poly_param_value_t&>( val);
}

void color_param_t::set_value( const Imath::Color4f& x, change_reason reason)
{
    float frame = 1.0f;

    if( composition())
        frame = composition()->frame();

    set_value_at_frame( x, frame, reason);
}

void color_param_t::set_value_at_frame( const Imath::Color4f& x, float frame, change_reason reason)
{
    if( can_undo())
        param_set()->add_command( this);

    poly_param_indexable_value_t v( x);
    value() = core::poly_cast<poly_param_value_t&>( v);

    bool autokey = param_set()->autokey();

    if( !is_static() && ( autokey || !curve( 0).empty()))
        curve( 0).insert( frame, x.r);

    if( !is_static() && ( autokey || !curve( 1).empty()))
        curve( 1).insert( frame, x.g);

    if( !is_static() && ( autokey || !curve( 2).empty()))
        curve( 2).insert( frame, x.b);

    if( is_rgba() && !is_static() && ( autokey || !curve( 3).empty()))
        curve( 3).insert( frame, x.a);

    if( composition_t * c = composition())
        evaluate( c->frame());

    emit_param_changed( reason);
}

void color_param_t::do_create_tracks( anim::track_t *parent)
{
    std::unique_ptr<anim::track_t> t( new anim::track_t( name()));

    std::unique_ptr<anim::track_t> tr( new anim::track_t( "R", &( curve( 0))));
    tr->changed.connect( std::bind( &animated_param_t::anim_curve_changed, this, std::placeholders::_1));
    t->add_child( std::move( tr));

    std::unique_ptr<anim::track_t> tg( new anim::track_t( "G", &( curve( 1))));
    tg->changed.connect( std::bind( &animated_param_t::anim_curve_changed, this, std::placeholders::_1));
    t->add_child( std::move( tg));

    std::unique_ptr<anim::track_t> tb( new anim::track_t( "B", &( curve( 2))));
    tb->changed.connect( std::bind( &animated_param_t::anim_curve_changed, this, std::placeholders::_1));
    t->add_child( std::move( tb));

    if( is_rgba())
    {
        std::unique_ptr<anim::track_t> ta( new anim::track_t( "A", &( curve( 3))));
        ta->changed.connect( std::bind( &animated_param_t::anim_curve_changed, this, std::placeholders::_1));
        t->add_child( std::move( ta));
    }

    parent->add_child( std::move( t));
}

void color_param_t::do_add_to_hash( hash::generator_t& hash_gen) const
{
    Imath::Color4f c = get_value<Imath::Color4f>( *this);
    hash_gen << c.r <<"," << c.g << "," << c.b;

    if( is_rgba())
        hash_gen << "," << c.a;
}

void color_param_t::do_read( const serialization::yaml_node_t& node)
{
    read_curves( node);

    Imath::Color4f val;
    if( node.get_optional_value( "value", val))
    {
        poly_param_indexable_value_t v( val);
        value().assign( core::poly_cast<poly_param_value_t&>( v));
    }
}

void color_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
    write_curves( out);
    bool one   = curve( 0).empty(); // && expression( 0).empty()
    bool two   = curve( 1).empty(); // && expression( 1).empty()
    bool three = curve( 2).empty(); // && expression( 2).empty()
    bool four  = curve( 3).empty() && is_rgba();  // && expression( 3).empty()

    if( one || two || three || four)
        out << YAML::Key << "value" << YAML::Value << get_value<Imath::Color4f>( *this);
}

} // namespace
