// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/image/generic/resize.hpp>

#include<algorithm>

#include<Imath/ImathBox.h>
#include<Imath/ImathMath.h>
#include<Imath/ImathFun.h>

#include<ramen/assert.hpp>

#include<ramen/image/generic/samplers.hpp>

namespace ramen
{
namespace image
{
namespace generic
{
namespace
{

float resize_coord( float x, float center, float scale)
{
	return ( ( x - center + 0.5f) / scale - 0.5f) + center;
}

} // unnamed

// util
Imath::V2i resize_point( const Imath::V2i& p, const Imath::V2i& center, float sx, float sy)
{
	return Imath::V2i(	std::floor( resize_coord( p.x, center.x, sx)),
						std::floor( resize_coord( p.y, center.y, sy)));
}

Imath::Box2i resize_box( const Imath::Box2i& box, const Imath::V2i& center, float sx, float sy)
{
	Imath::V2i p(	std::floor( ( box.min.x - center.x) / sx) + center.x,
					std::floor( ( box.min.y - center.y) / sy) + center.y);

	Imath::V2i q(	std::floor( ( box.max.x - center.x) / sx) + center.x,
					std::floor( ( box.max.y - center.y) / sy) + center.y);

	return Imath::Box2i( p, q);
}

} // namespace
} // namespace
} // namespace
