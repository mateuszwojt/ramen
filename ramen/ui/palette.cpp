// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/palette.hpp>

#include<QApplication>
#include<QPalette>

namespace ramen
{
namespace ui
{

void init_palette()
{
	QColor bgcol = QApplication::palette().color( QPalette::Base);
	QColor fgcol = QApplication::palette().color( QPalette::Text);

	palette_t::instance().set_color( "background", Imath::Color3c( bgcol.red(), bgcol.green(), bgcol.blue()));
	palette_t::instance().set_color( "text", Imath::Color3c( fgcol.red(), fgcol.green(), fgcol.blue()));
}

QColor palette_qcolor( const std::string& k)
{
	Imath::Color3c c = palette_t::instance().color( k);
	return QColor( c.x, c.y, c.z, 255);
}

} // ui
} // ramen
