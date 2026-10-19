// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/image/input/image_channels_param.hpp>

#include<algorithm>

#include<ramen/app/application.hpp>

#include<ramen/params/param_set.hpp>

namespace ramen
{

image_channels_param_t::image_channels_param_t() : static_param_t( std::string())
{
	value().assign( tuple4i_t( 0, 1, 2, 3));
}

image_channels_param_t::image_channels_param_t( const image_channels_param_t& other) : static_param_t( other)
{
	channel_list_ = other.channel_list_;
}

void image_channels_param_t::clear_channel_list()
{
	channel_list_.clear();
	value().assign( tuple4i_t( 0, 1, 2, 3));

	update_widgets();
}

void image_channels_param_t::set_channel_list( const std::vector<std::string>& channels)
{
	if( !channel_list_.empty())
	{
		std::string ch0, ch1, ch2, ch3;
		get_channel_names( ch0, ch1, ch2, ch3);
		channel_list_ = channels;
		set_channels( ch0, ch1, ch2, ch3);
	}
	else
	{
		channel_list_ = channels;
		value().assign( tuple4i_t( 0, 1, 2, 3));
	}

	tuple4i_t ch( get_value<tuple4i_t>( *this));

	update_widgets();
}

void image_channels_param_t::get_channel_names( std::string& red, std::string& green,
												std::string& blue, std::string& alpha) const
{
	tuple4i_t ch( get_value<tuple4i_t>( *this));
	
	if( std::get<0>( ch) < channel_list_.size())
		red = channel_list_[ std::get<0>( ch)];
	else
		red = "R";
	
	if( std::get<1>( ch) < channel_list_.size())
		green = channel_list_[ std::get<1>( ch)];
	else
		green = "G";

	if( std::get<2>( ch) < channel_list_.size())
		blue = channel_list_[ std::get<2>( ch)];
	else
		blue = "B";

	if( std::get<3>( ch) < channel_list_.size())
		alpha = channel_list_[ std::get<3>( ch)];
	else
		alpha = "A";
}

void image_channels_param_t::set_channels( const tuple4i_t& ch)
{ 
	value().assign( ch);
}

void image_channels_param_t::set_channels( const std::string& red, const std::string& green,
											const std::string& blue, const std::string& alpha)
{
	RAMEN_ASSERT( !channel_list_.empty());
	
    int c0, c1, c2, c3;
    std::vector<std::string>::const_iterator it;

    // red
    it = std::ranges::find( channel_list_, red);

    if( it != channel_list_.end())
		c0 = it - channel_list_.begin();
    else
		c0 = channel_list_.size();

    // green
    it = std::ranges::find( channel_list_, green);

    if( it != channel_list_.end())
		c1 = it - channel_list_.begin();
    else
		c1 = channel_list_.size();

    // blue
    it = std::ranges::find( channel_list_, blue);

    if( it != channel_list_.end())
		c2 = it - channel_list_.begin();
    else
		c2 = channel_list_.size();

    // alpha
    it = std::ranges::find( channel_list_, alpha);

    if( it != channel_list_.end())
		c3 = it - channel_list_.begin();
    else
		c3 = channel_list_.size();

    tuple4i_t ch( c0, c1, c2, c3);
    value().assign( ch);
}

// ui
} // namespace
