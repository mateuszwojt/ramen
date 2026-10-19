// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_FLOAT3_PARAM_HPP
#define	RAMEN_FLOAT3_PARAM_HPP

#include<ramen/params/proportional_param.hpp>

#include<Imath/ImathVec.h>

namespace ramen
{

namespace ui
{
class float3_param_widget_t;
} // ui

class RAMEN_API float3_param_t : public proportional_param_t
{
    friend class ui::float3_param_widget_t;

public:

    explicit float3_param_t( const std::string& name);

    void set_default_value( const Imath::V3f& x);

    virtual poly_param_value_t value_at_frame( float frame) const;

    void set_value( const Imath::V3f& x, change_reason reason = user_edited);
    void set_value_at_frame( const Imath::V3f& x, float frame, change_reason reason = user_edited);

    Imath::V3f derive( float time) const;
    Imath::V3f integrate( float time1, float time2) const;

protected:

    float3_param_t( const float3_param_t& other);
    void operator=( const float3_param_t& other);

private:

    void private_init();

    virtual param_t *do_clone() const { return new float3_param_t( *this);}


    virtual void do_add_to_hash( hash::generator_t& hash_gen) const;

    virtual void do_read( const serialization::yaml_node_t& node);
    virtual void do_write( serialization::yaml_oarchive_t& out) const;

};

} // namespace

#endif
