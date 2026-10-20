// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/GL/glu.hpp>

namespace ramen
{

void glu_ortho2d( double left, double right, double bottom, double top) { gl_ortho( left, right, bottom, top, -1.0, 1.0);}

} // ramen
