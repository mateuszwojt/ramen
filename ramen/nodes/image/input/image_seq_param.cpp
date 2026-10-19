// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/image/input/image_seq_param.hpp>

#include<filesystem>


#include<ramen/app/application.hpp>

namespace ramen
{

image_seq_param_t::image_seq_param_t( const std::string& name, int proxy_level) : static_param_t( name)
{
	proxy_level_ = proxy_level;
}

image_seq_param_t::image_seq_param_t( const image_seq_param_t& other) : static_param_t( other)
{
	proxy_level_ = other.proxy_level_;
	input_text_ = other.input_text_;
}

void image_seq_param_t::set_input_text( const std::string& str)
{
	input_text_ = str;
	update_widgets();
}

} // namespace
