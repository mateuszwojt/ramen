// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_APP_PALETTE_HPP
#define RAMEN_APP_PALETTE_HPP

#include<ramen/config.hpp>

#include<map>
#include<string>

#include<Imath/ImathColor.h>

#include<ramen/core/noncopyable.hpp>

namespace ramen
{

/*!
\ingroup app
\brief Named colors, like the node plug colors.
*/
class RAMEN_API palette_t : ramen::core::noncopyable_t
{
public:

    static palette_t& instance();

    /// Returns the color named k, or a magenta error color if it does not exist.
    const Imath::Color3c& color( const std::string& k) const;

	bool find_color( const std::string& k) const;

	void set_color( const std::string& k, const Imath::Color3c& c);

private:

    palette_t();

    std::map<std::string, Imath::Color3c> colors_;
	Imath::Color3c error_color_;
};

} // ramen

#endif
