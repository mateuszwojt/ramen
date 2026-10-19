// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PALETTE_HPP
#define RAMEN_UI_PALETTE_HPP

#include<ramen/config.hpp>

#include<string>

#include<QColor>

#include<ramen/app/palette.hpp>

namespace ramen
{
namespace ui
{

/// Sets the palette background and text colors from the application style.
RAMEN_API void init_palette();

/// Returns a palette color as a QColor.
RAMEN_API QColor palette_qcolor( const std::string& k);

} // ui
} // ramen

#endif
