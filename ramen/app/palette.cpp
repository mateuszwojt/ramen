// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/app/palette.hpp>

#ifndef NDEBUG
	#include<iostream>
#endif

namespace ramen
{

palette_t& palette_t::instance()
{
    static palette_t pal;
    return pal;
}

palette_t::palette_t()
{
	error_color_ = Imath::Color3c( 255, 0, 255);

	// the user interface replaces these with the ones in its style.
	colors_["background"]   = Imath::Color3c( 42, 42, 42);
	colors_["text"]         = Imath::Color3c( 220, 220, 220);

	colors_["front plug"]   = Imath::Color3c( 255, 0, 0);
	colors_["back plug"]    = Imath::Color3c( 0, 255, 0);
	colors_["matte plug"]   = Imath::Color3c( 0, 0, 255);
	colors_["out plug"]	    = Imath::Color3c( 255, 255, 0);
	colors_["optional plug"] = Imath::Color3c( 0, 255, 0);

	colors_["node_shadow"]		= Imath::Color3c( 64, 64, 64);
	colors_["node_bg"]			= Imath::Color3c( 150, 150, 150);
	colors_["node_picked_bg"]	= Imath::Color3c( 200, 140, 0);
	colors_["node_error_bg"]	= Imath::Color3c( 150, 25, 0);

	colors_["node_active_out"]  = Imath::Color3c( 255, 0, 0);
	colors_["node_context_out"] = Imath::Color3c( 0, 0, 255);

	// manipulators
	colors_["manipulator"]			= Imath::Color3c( 255, 255, 0);
	colors_["active_manipulator"]	= Imath::Color3c( 255, 0, 0);

	colors_["x_axis"] = Imath::Color3c( 255, 0, 0);
	colors_["y_axis"] = Imath::Color3c( 0, 255, 0);
	colors_["z_axis"] = Imath::Color3c( 0, 0, 255);

	// interface elems
	colors_["grid"] = Imath::Color3c( 132, 132, 132);
	colors_["box_pick"] = Imath::Color3c( 255, 0, 0);
}

const Imath::Color3c& palette_t::color( const std::string& k) const
{
	std::map<std::string, Imath::Color3c>::const_iterator it( colors_.find( k));

	if( it != colors_.end())
		return it->second;

	#ifndef NDEBUG
		std::cout << "Palette: color not found " << k << "\n";
	#endif

	return error_color_;
}

bool palette_t::find_color( const std::string& k) const { return colors_.find( k) != colors_.end();}

void palette_t::set_color( const std::string& k, const Imath::Color3c& c) { colors_[k] = c;}

} // ramen
