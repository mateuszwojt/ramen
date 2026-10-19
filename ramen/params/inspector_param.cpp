// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/inspector_param.hpp>

#include<functional>

#include<ramen/params/parameterised.hpp>

namespace ramen
{

inspector_param_t::inspector_param_t( const std::string& id) : param_t()
{
	set_static( true);
	set_id( id);
	contents_ = 0;
}

inspector_param_t::inspector_param_t( const inspector_param_t& other) : param_t( other), contents_( 0)
{
}

inspector_param_t::~inspector_param_t() { connection_.disconnect();}

void inspector_param_t::set_parameterised( parameterised_t *p)
{
	if( contents_)
	{
		connection_.disconnect();
		contents_ = 0;
	}

	if( p)
	{
		contents_ = p;
		connection_ = p->deleted.connect( std::bind( &inspector_param_t::parameterised_deleted, this, std::placeholders::_1));
	}

	contents_changed();
}

void inspector_param_t::parameterised_deleted( parameterised_t *p)
{
	if( p == contents_)
		set_parameterised( 0);
}

} // namespace
