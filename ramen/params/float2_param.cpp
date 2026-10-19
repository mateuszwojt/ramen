// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/float2_param.hpp>

#include<limits>

#include<functional>


#include<ramen/app/application.hpp>
#include<ramen/app/ui_notifications.hpp>
#include<ramen/app/document.hpp>

#include<ramen/nodes/node.hpp>

#include<ramen/algorithm/clamp.hpp>


namespace ramen
{

float2_param_t::float2_param_t( const std::string& name) : proportional_param_t( name) { private_init();}

float2_param_t::float2_param_t( const float2_param_t& other) : proportional_param_t( other)
{
}

void float2_param_t::private_init()
{
    add_curve( "X");
    add_curve( "Y");
    set_default_value( Imath::V2f( 0, 0));
}

void float2_param_t::set_default_value( const Imath::V2f& x)
{
    poly_param_indexable_value_t v( x);
    value() = core::poly_cast<poly_param_value_t&>( v);
}

poly_param_value_t float2_param_t::value_at_frame( float frame) const
{
    Imath::V2f v( get_value<Imath::V2f>( *this));
    eval_curve( 0, frame, v.x);
    eval_curve( 1, frame, v.y);
    poly_param_indexable_value_t val( v);
    return core::poly_cast<poly_param_value_t&>( val);
}

void float2_param_t::set_value( const Imath::V2f& x, change_reason reason)
{
    float frame = 1.0f;

    if( composition_t * c = composition())
        frame = c->frame();

    set_value_at_frame( x, frame, reason);
}

void float2_param_t::set_value_at_frame( const Imath::V2f& x, float frame, change_reason reason)
{
    if( can_undo())
        param_set()->add_command( this);

    poly_param_indexable_value_t v( x);
    value() = core::poly_cast<poly_param_value_t&>( v);

    bool autokey = param_set()->autokey();

    if( !is_static() && ( autokey || !curve( 0).empty()))
        curve( 0).insert( frame, x.x);

    if( !is_static() && ( autokey || !curve( 1).empty()))
        curve( 1).insert( frame, x.y);

    if( composition_t * c = composition())
        evaluate( c->frame());

    emit_param_changed( reason);
}

void float2_param_t::set_absolute_value( const Imath::V2f& x, change_reason reason)
{
    set_value( absolute_to_relative( round( x)), reason);
}

void float2_param_t::set_absolute_value_at_frame( const Imath::V2f& x, float frame, change_reason reason)
{
    set_value_at_frame( absolute_to_relative( round( x)), frame, reason);
}

Imath::V2f float2_param_t::derive( float time) const
{
    Imath::V2f result( 0.0f, 0.0f);

    if( !curve( 0).empty())
        result.x = curve( 0).derive( time);

    if( !curve( 1).empty())
        result.y = curve( 1).derive( time);

    return result;
}

Imath::V2f float2_param_t::integrate( float time1, float time2) const
{
    Imath::V2f result = get_value<Imath::V2f>( *this);
    result.x  = (time2 - time1) * result.x;
    result.y  = (time2 - time1) * result.y;

    if( !curve( 0).empty())
        result.x = curve( 0).integrate( time1, time2);

    if( !curve( 1).empty())
        result.y = curve( 1).integrate( time1, time2);

    return result;
}

void float2_param_t::do_format_changed( const Imath::Box2i& new_domain, float aspect, const Imath::V2f& proxy_scale)
{
    Imath::V2f scale, offset;
    get_scale_and_offset( scale, offset);
    curve( 0).set_scale( scale.x);
    curve( 0).set_offset( offset.x);
    curve( 1).set_scale( scale.y);
    curve( 1).set_offset( offset.y);
    ui_notifications().update_anim_editors();

    update_widgets();
}

void float2_param_t::do_add_to_hash( hash::generator_t& hash_gen) const
{
    Imath::V2f v( get_value<Imath::V2f>( *this));
    hash_gen << v.x << "," << v.y;
}

void float2_param_t::do_read( const serialization::yaml_node_t& node)
{
    read_curves( node);

    Imath::V2f val;
    if( node.get_optional_value( "value", val))
    {
        poly_param_indexable_value_t v( val);
        value().assign( core::poly_cast<poly_param_value_t&>( v));
    }
}

void float2_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
    write_curves( out);

    bool one = curve( 0).empty(); // && expression( 0).empty();
    bool two = curve( 1).empty(); // && expression( 1).empty();

    if( one || two)
        out << YAML::Key << "value" << YAML::Value << get_value<Imath::V2f>( *this);
}

} // namespace
