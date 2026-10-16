// This code comes from the procedural texturing an modelling book.
// Add a proper copyright here.

#ifndef RAMEN_NOISE_PERLIN_HPP
#define RAMEN_NOISE_PERLIN_HPP

#include<ramen/core/noncopyable.hpp>

#include<ramen/noise/noise_base.hpp>

namespace ramen
{
namespace noise
{

class perlin_t : public noise_base_t, ramen::core::noncopyable_t
{
public:

	perlin_t( int seed = 0);

	float operator()( float x) const;
	float operator()( float x, float y) const;
	float operator()( float x, float y, float z) const;
	float operator()( const Imath::V2f& v) const { return (*this)( v.x, v.y);}
	float operator()( const Imath::V3f& v) const { return (*this)( v.x, v.y, v.z);}
	
private:

	void init( int seed);

	float do_noise1( float arg) const;
	float do_noise2( float vec[2]) const;	
	float do_noise3( float vec[3]) const;
	
	float remap( float x) const;
	
	void normalize2(float v[2]) const;
	void normalize3(float v[3]) const;

	static constexpr int B = 0x100;
	static constexpr int BM = 0xff;
	static constexpr int N = 0x1000;
	static constexpr int NP = 12;
	static constexpr int NM = 0xfff;

	int p[B + B + 2];
	float g3[B + B + 2][3];
	float g2[B + B + 2][2];
	float g1[B + B + 2];
};
	
} // noise
} // ramen

#endif
