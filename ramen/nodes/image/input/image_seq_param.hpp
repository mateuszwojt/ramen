// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_IMAGE_SEQ_PARAM_HPP
#define	RAMEN_IMAGE_SEQ_PARAM_HPP

#include<ramen/params/static_param.hpp>

#include<ramen/core/signal.hpp>

#include<ramen/filesystem/path.hpp>

namespace ramen
{

namespace ui
{
class image_seq_param_widget_t;
} // ui

class image_seq_param_t : public static_param_t
{
    friend class ui::image_seq_param_widget_t;

public:

    image_seq_param_t( const std::string& name, int proxy_level);

    core::signal_t<void ( const::std::filesystem::path&, int, bool, bool)> file_picked;

	void set_input_text( const std::string& str);
	
protected:

    image_seq_param_t( const image_seq_param_t& other);
    void operator=( const image_seq_param_t& other);
	
private:

    virtual param_t *do_clone() const { return new image_seq_param_t( *this);}
	

	int proxy_level_;
	std::string input_text_;
};

} // namespace

#endif
