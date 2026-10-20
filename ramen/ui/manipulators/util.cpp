// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/manipulators/util.hpp>

#include<Imath/ImathMath.h>

#include<ramen/math/constants.hpp>

namespace ramen
{
namespace manipulators
{

Imath::V2f rotate_point( const Imath::V2f& center, const Imath::V2f& p, float angle_in_degrees)
{
	float cs = std::cos( math::constants<float>::deg2rad() * angle_in_degrees);
	float ss = std::sin( math::constants<float>::deg2rad() * angle_in_degrees);
	
	Imath::V2f q( p - center);
	Imath::V2f r;
	r.x = q.x * cs - q.y * ss;
	r.y = q.x * ss + q.y * cs;
	return r + center;
}
	
} // namespace
} // namespace
