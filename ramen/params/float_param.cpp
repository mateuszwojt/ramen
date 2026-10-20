// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/float_param.hpp>

#include<limits>

#include<functional>

#include<ramen/app/application.hpp>
#include<ramen/app/ui_notifications.hpp>
#include<ramen/app/composition.hpp>

#include<ramen/params/parameterised.hpp>


#include<ramen/serialization/yaml_node.hpp>
#include<ramen/serialization/yaml_oarchive.hpp>


namespace ramen
{

float_param_t::float_param_t( const std::string& name) : numeric_param_t( name) { private_init();}
float_param_t::float_param_t( const float_param_t& other) : numeric_param_t( other)
{
}

void float_param_t::private_init()
{
    add_curve( name());
    set_default_value( 0);
}

poly_param_value_t float_param_t::value_at_frame( float frame) const
{
    float val = get_value<float>( *this);
    eval_curve( 0, frame, val);
    return poly_param_value_t( val);
}

void float_param_t::set_default_value( float x) { value().assign( x);}

void float_param_t::set_value( float x, change_reason reason)
{
    float frame = 1.0;

    if( composition_t * c = composition())
        frame = c->frame();

    set_value_at_frame( x, frame, reason);
}

void float_param_t::set_value_at_frame( float x, float frame, change_reason reason)
{
    if( can_undo())
        param_set()->add_command( this);

    if( !is_static() && ( param_set()->autokey() || !curve().empty()))
    {
        curve().insert( frame, x);

        if( composition_t * c = composition())
            evaluate( c->frame());
    }
    else
        value().assign( x);

    emit_param_changed( reason);
}

void float_param_t::set_absolute_value( float x, change_reason reason)
{
    set_value( absolute_to_relative( round( x)), reason);
}

void float_param_t::set_absolute_value_at_frame( float x, float frame, change_reason reason)
{
    set_value_at_frame( absolute_to_relative( round( x)), frame, reason);
}

float float_param_t::derive( float time) const
{
    if( num_curves() && !curve().empty())
        return curve().derive( time);
    else
        return 0;
}

float float_param_t::integrate( float time1, float time2) const
{
    if( num_curves() && !curve().empty())
        return curve().integrate( time1, time2);
    else
    {
        float v = get_value<float>( *this);
        return (time2 - time1) * v;
    }
}

void float_param_t::do_format_changed( const Imath::Box2i& new_domain, float aspect, const Imath::V2f& proxy_scale)
{
    if( num_curves())
    {
        float scale, offset;
        get_scale_and_offset( scale, offset);
        curve().set_scale( scale);
        curve().set_offset( offset);
        ui_notifications().update_anim_editors();
    }

    update_widgets();
}

void float_param_t::do_add_to_hash( hash::generator_t& hash_gen) const
{
    hash_gen << get_value<float>( *this);
}

void float_param_t::do_read( const serialization::yaml_node_t& node)
{
    serialization::optional_yaml_node_t n = node.get_optional_node( "curve");

    if( n)
    {
        curve().read( n.value());
        return;
    }

    float val = 0;
    node.get_value<float>( "value", val);
    value().assign( val);
}

void float_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
    {
        if( !curve().empty())
        {
            out << YAML::Key << "curve" << YAML::Value;
            curve().write( out);
        }
        else
            out << YAML::Key << "value" << YAML::Value << get_value<float>( *this);
    }
}

} // namespace
