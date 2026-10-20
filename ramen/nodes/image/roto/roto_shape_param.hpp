// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_ROTO_SHAPE_PARAM_HPP
#define	RAMEN_ROTO_SHAPE_PARAM_HPP

#include<ramen/config.hpp>

#include<ramen/params/static_param.hpp>

#include<ramen/nodes/image/roto/roto_node_fwd.hpp>
#include<ramen/nodes/image/roto/shape_fwd.hpp>

namespace ramen
{

namespace ui
{
class roto_shape_param_widget_t;
} // ui

class roto_shape_param_t : public static_param_t
{
    friend class ui::roto_shape_param_widget_t;

public:

    roto_shape_param_t( const std::string& id);
	
	void set_active_shape( roto::shape_t *s);

	void shape_list_changed();

protected:

    roto_shape_param_t( const roto_shape_param_t& other);
    void operator=( const roto_shape_param_t& other);

private:

    virtual param_t *do_clone() const;

	
	const image::roto_node_t& roto_node() const;
	image::roto_node_t& roto_node();

	
	roto::shape_t *selected_;
};

} // namespace

#endif
