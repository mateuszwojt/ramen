// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/image/roto/roto_shape_param.hpp>


#include<ramen/assert.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/undo/stack.hpp>

#include<ramen/nodes/image/roto/roto_node.hpp>
#include<ramen/nodes/image/roto/shape.hpp>
#include<ramen/nodes/image/roto/roto_commands.hpp>

namespace ramen
{

roto_shape_param_t::roto_shape_param_t( const std::string& id) : static_param_t( "Shape"), selected_( 0)
{
	set_id( id);
}

roto_shape_param_t::roto_shape_param_t( const roto_shape_param_t& other) : static_param_t( other)
{
	selected_ = 0;
}

param_t *roto_shape_param_t::do_clone() const { return new roto_shape_param_t( *this);}

void roto_shape_param_t::set_active_shape( roto::shape_t *s)
{
	selected_ = s;
	update_widgets();
}

void roto_shape_param_t::shape_list_changed() { update_widgets();}

const image::roto_node_t& roto_shape_param_t::roto_node() const
{
	const image::roto_node_t *r = dynamic_cast<const image::roto_node_t*>( parameterised());
	RAMEN_ASSERT( r);
	
	return *r;
}

image::roto_node_t& roto_shape_param_t::roto_node()
{
	image::roto_node_t *r = dynamic_cast<image::roto_node_t*>( parameterised());
	RAMEN_ASSERT( r);
	
	return *r;
}

} // namespace
