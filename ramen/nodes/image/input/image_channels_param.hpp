// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_IMAGE_CHANNELS_PARAM_HPP
#define	RAMEN_IMAGE_CHANNELS_PARAM_HPP

#include<ramen/config.hpp>

#include<ramen/params/static_param.hpp>

#include<vector>

#include<tuple>

namespace ramen
{

namespace ui
{
class image_channels_param_widget_t;
} // ui

typedef std::tuple<int,int,int,int> tuple4i_t;
	
class image_channels_param_t : public static_param_t
{
    friend class ui::image_channels_param_widget_t;

public:

    explicit image_channels_param_t();

	void clear_channel_list();
	void set_channel_list( const std::vector<std::string>& channels);
	
	void get_channel_names( std::string& red, std::string& green,
							std::string& blue, std::string& alpha) const;
	
	void set_channels( const tuple4i_t& ch);
	
	void set_channels( const std::string& red, const std::string& green,
						const std::string& blue, const std::string& alpha);
	
protected:

    image_channels_param_t( const image_channels_param_t& other);
    void operator=( const image_channels_param_t& other);

private:

    virtual param_t *do_clone() const { return new image_channels_param_t( *this);}

	
	std::vector<std::string> channel_list_;
};

} // namespace

#endif
