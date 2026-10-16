// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/image/processing.hpp>

#include<ramen/image/generic/affine_warp.hpp>
#include<ramen/image/generic/alpha_composite.hpp>
#include<ramen/image/generic/apply_color_matrix.hpp>
#include<ramen/image/generic/apply_gamma_exp_log.hpp>
#include<ramen/image/generic/box_blur_channel.hpp>
#include<ramen/image/generic/composite.hpp>
#include<ramen/image/generic/convolve33.hpp>
#include<ramen/image/generic/gaussian_blur_channel.hpp>
#include<ramen/image/generic/gaussian_blur_rgb.hpp>
#include<ramen/image/generic/invert.hpp>
#include<ramen/image/generic/math.hpp>
#include<ramen/image/generic/premultiply.hpp>
#include<ramen/image/generic/projective_warp.hpp>
#include<ramen/image/generic/resize.hpp>
#include<ramen/image/generic/rgb_to_yuv.hpp>
#include<ramen/image/generic/rgb_to_hsv.hpp>
#include<ramen/image/generic/rgb_to_lab.hpp>
#include<ramen/image/generic/rgb_to_gray.hpp>
#include<ramen/image/generic/stats.hpp>

#include<ramen/image/sse2/composite.hpp>
#include<ramen/image/sse2/affine_warp.hpp>
#include<ramen/image/sse2/box_blur.hpp>
#include<ramen/image/sse2/gaussian_blur.hpp>
#include<ramen/image/sse2/key_mix.hpp>
#include<ramen/image/sse2/math.hpp>
#include<ramen/image/sse2/projective_warp.hpp>
#include<ramen/image/sse2/resize.hpp>
#include<ramen/image/sse2/warp.hpp>

namespace ramen
{
namespace image
{	
	
// basic math
std::function<void ( const image_view_t&)> clear_image = generic::clear_image;
std::function<void ( const const_image_view_t&, const const_image_view_t&, const image_view_t&)> add_images = sse2::add_images;
std::function<void ( const const_image_view_t&, const const_image_view_t&, const image_view_t&)> sub_images = sse2::sub_images;
std::function<void ( const const_image_view_t&, const const_image_view_t&, const image_view_t&)> mul_images = sse2::mul_images;
std::function<void ( const const_image_view_t&, float, const image_view_t&)> mul_image_scalar = sse2::mul_image_scalar;
std::function<void ( const const_image_view_t&, const const_image_view_t&, float, const image_view_t&)> madd_images = sse2::madd_images;
std::function<void ( const const_image_view_t&, const const_image_view_t&, float, const image_view_t&)> lerp_images = sse2::lerp_images;

std::function<void ( const channel_view_t&)> clear_channel= generic::clear_channel;
std::function<void ( const const_channel_view_t&, const const_channel_view_t&, const channel_view_t&)> add_channels = generic::add_channels;
std::function<void ( const const_channel_view_t&, const const_channel_view_t&, const channel_view_t&)> sub_channels = generic::sub_channels;
std::function<void ( const const_channel_view_t&, const const_channel_view_t&, const channel_view_t&)> mul_channels = generic::mul_channels;
std::function<void ( const const_channel_view_t&, const const_channel_view_t&, float, const channel_view_t&)> madd_channels = generic::madd_channels;
std::function<void ( const const_channel_view_t&, float, const channel_view_t&)> mul_channel_scalar = generic::mul_channel_scalar;

std::function<void ( const gray_image_view_t&)> clear_gray_image;
std::function<void ( const const_gray_image_view_t&, const const_gray_image_view_t&, const gray_image_view_t&)> add_gray_images = generic::add_gray_images;
std::function<void ( const const_gray_image_view_t&, const const_gray_image_view_t&, const gray_image_view_t&)> sub_gray_images = generic::sub_gray_images;
std::function<void ( const const_gray_image_view_t&, const const_gray_image_view_t&, const gray_image_view_t&)> mul_gray_images = generic::mul_gray_images;
std::function<void ( const const_gray_image_view_t&, float, const gray_image_view_t&)> mul_gray_image_scalar = generic::mul_gray_image_scalar;
std::function<void ( const const_gray_image_view_t&, const const_gray_image_view_t&, float, const gray_image_view_t&)> madd_gray_images = generic::madd_gray_images;
std::function<void ( const const_gray_image_view_t&, const const_gray_image_view_t&, float, const gray_image_view_t&)> lerp_gray_images = generic::lerp_gray_images;

// channels
std::function<void ( const const_image_view_t&, const image_view_t&)> premultiply = generic::premultiply;
std::function<void ( const const_image_view_t&, const image_view_t&)> unpremultiply = generic::unpremultiply;

std::function<void ( const const_image_view_t&, const image_view_t&)> invert_rgba = generic::invert_rgba;
std::function<void ( const const_image_view_t&, const image_view_t&)> invert_rgb = generic::invert_rgb;
std::function<void ( const const_image_view_t&, const image_view_t&)> invert_alpha = generic::invert_alpha;
std::function<void ( const const_channel_view_t&, const channel_view_t&)> invert_channel = generic::invert_channel;

// color space
std::function<void ( const const_image_view_t&, const image_view_t&)> convert_rgb_to_yuv = generic::convert_rgb_to_yuv;
std::function<void ( const const_image_view_t&, const image_view_t&)> convert_rgb_to_hsv = generic::convert_rgb_to_hsv;
std::function<void ( const const_image_view_t&, const image_view_t&)> convert_rgb_to_lab = generic::convert_rgb_to_lab;

std::function<void ( const const_image_view_t&, const image_view_t&)> convert_yuv_to_rgb = generic::convert_yuv_to_rgb;
std::function<void ( const const_image_view_t&, const image_view_t&)> convert_hsv_to_rgb = generic::convert_hsv_to_rgb;
std::function<void ( const const_image_view_t&, const image_view_t&)> convert_lab_to_rgb = generic::convert_lab_to_rgb;

std::function<void ( const const_image_view_t&, const gray_image_view_t&)> convert_rgb_to_gray = generic::convert_rgb_to_gray;

// color
std::function<void ( const const_image_view_t&, const image_view_t&, float)> apply_gamma = generic::apply_gamma;
std::function<void ( const const_image_view_t&, const image_view_t&)> apply_log = generic::apply_log;
std::function<void ( const const_image_view_t&, const image_view_t&)> apply_pow10 = generic::apply_pow10;
std::function<void ( const const_image_view_t&, const image_view_t&, const matrix4_t&)> apply_color_matrix = generic::apply_color_matrix;

// stats
std::function<image::pixel_t ( const const_image_view_t&)> mean = generic::mean;

// transform
std::function<void ( const Imath::Box2i&, const const_image_view_t&,
		     const Imath::Box2i&, const image_view_t&,
		     const matrix3_t&,
		    const matrix3_t&)> affine_warp_nearest = generic::affine_warp_nearest;

std::function<void ( const Imath::Box2i&, const const_image_view_t&,
		     const Imath::Box2i&, const image_view_t&,
		     const matrix3_t&,
		    const matrix3_t&)> affine_warp_bilinear = sse2::affine_warp_bilinear;

std::function<void ( const Imath::Box2i&, const const_image_view_t&,
		     const Imath::Box2i&, const image_view_t&,
		     const matrix3_t&,
		    const matrix3_t&)> affine_warp_catrom = sse2::affine_warp_catrom;

std::function<void ( const Imath::Box2i&, const const_image_view_t&,
		     const Imath::Box2i&, const image_view_t&,
		     const matrix3_t&,
		    const matrix3_t&)> projective_warp_nearest = generic::projective_warp_nearest;

std::function<void ( const Imath::Box2i&, const const_image_view_t&,
		     const Imath::Box2i&, const image_view_t&,
		     const matrix3_t&,
		    const matrix3_t&)> projective_warp_bilinear = sse2::projective_warp_bilinear;

std::function<void ( const Imath::Box2i&, const const_image_view_t&,
		     const Imath::Box2i&, const image_view_t&,
		     const matrix3_t&,
		    const matrix3_t&)> projective_warp_catrom = sse2::projective_warp_catrom;

// transform + non black border modes
std::function<void ( const Imath::Box2i&, const const_image_view_t&,
		     const Imath::Box2i&, const image_view_t&,
		     const matrix3_t&,
		    const matrix3_t&)> affine_warp_bilinear_tile = sse2::affine_warp_bilinear_tile;

std::function<void ( const Imath::Box2i&, const const_image_view_t&,
		     const Imath::Box2i&, const image_view_t&,
		     const matrix3_t&,
		    const matrix3_t&)> affine_warp_bilinear_mirror = sse2::affine_warp_bilinear_mirror;

// resize

std::function<void ( const const_image_view_t&, const image_view_t&)> resize_bilinear = sse2::resize_bilinear;

std::function<void ( const image::const_image_view_t&, const Imath::Box2i&,
					   const image::image_view_t&, const Imath::Box2i&)> resize_half = sse2::resize_half;

std::function<void ( const image::const_image_view_t&, const Imath::Box2i&, const Imath::Box2i&,
					   const image::image_view_t&, const Imath::Box2i&, const image::image_view_t&,
					  const Imath::Box2i&, const Imath::V2i&, const Imath::V2f&)> resize_lanczos3_ = sse2::resize_lanczos3;

std::function<void ( const image::const_image_view_t&, const Imath::Box2i&, const Imath::Box2i&,
					   const image::image_view_t&, const Imath::Box2i&, const image::image_view_t&,
					  const Imath::Box2i&, const Imath::V2i&, const Imath::V2f&)> resize_mitchell_ = sse2::resize_mitchell;

std::function<void ( const image::const_image_view_t&, const Imath::Box2i&, const Imath::Box2i&,
					   const image::image_view_t&, const Imath::Box2i&, const image::image_view_t&,
					  const Imath::Box2i&, const Imath::V2i&, const Imath::V2f&)> resize_catrom_ = sse2::resize_catrom;

// composite
std::function<void ( const const_image_view_t&, const const_image_view_t&,
				const image_view_t&, float)> composite_over = sse2::composite_over;

std::function<void ( const const_image_view_t&, const const_image_view_t&,
				const image_view_t&, float)> composite_add = sse2::composite_add;

std::function<void ( const const_image_view_t&, const const_image_view_t&,
				const image_view_t&, float)> composite_mul = sse2::composite_mul;

std::function<void ( const const_image_view_t&, const const_image_view_t&,
				const image_view_t&, float)> composite_sub = sse2::composite_sub;

std::function<void ( const const_image_view_t&, const const_image_view_t&,
				const image_view_t&, float)> composite_mix = sse2::composite_mix;

std::function<void ( const const_image_view_t&, const const_image_view_t&,
				const image_view_t&, float)> composite_screen = generic::composite_screen;

std::function<void ( const const_image_view_t&, const const_image_view_t&,
				const image_view_t&, float)> composite_overlay = generic::composite_overlay;

std::function<void ( const const_image_view_t&, const image_view_t&,
                                                float)> composite_zero_overlay = generic::composite_zero_overlay;

std::function<void ( const const_image_view_t&, const const_image_view_t&,
				const image_view_t&, float)> composite_diff = generic::composite_diff;

std::function<void ( const const_image_view_t&, const const_image_view_t&,
				const image_view_t&, float)> composite_max = sse2::composite_max;

std::function<void ( const const_image_view_t&, const const_image_view_t&,
				const image_view_t&, float)> composite_min = sse2::composite_min;

std::function<void ( const const_image_view_t&, const const_image_view_t&, const image_view_t&, float)> alpha_composite_add = generic::alpha_composite_add;
std::function<void ( const const_image_view_t&, const const_image_view_t&, const image_view_t&, float)> alpha_composite_mul = generic::alpha_composite_mul;
std::function<void ( const const_image_view_t&, const const_image_view_t&, const image_view_t&, float)> alpha_composite_sub = generic::alpha_composite_sub;
std::function<void ( const const_image_view_t&, const const_image_view_t&, const image_view_t&, float)> alpha_composite_mix = generic::alpha_composite_mix;
std::function<void ( const const_image_view_t&, const const_image_view_t&, const image_view_t&, float)> alpha_composite_max = generic::alpha_composite_max;
std::function<void ( const const_image_view_t&, const const_image_view_t&, const image_view_t&, float)> alpha_composite_min = generic::alpha_composite_min;

// warps
std::function<void ( const Imath::Box2i&, const const_image_view_t&,
				const Imath::Box2i&, const image_view_t&,
				const warp_function_t&, bool, bool)> warp_bilinear = sse2::warp_bilinear;

std::function<void ( const Imath::Box2i&, const const_image_view_t&,
				const Imath::Box2i&, const image_view_t&,
				const warp_function_t&, bool, bool)> warp_bilinear_tile = sse2::warp_bilinear_tile;

std::function<void ( const Imath::Box2i&, const const_image_view_t&,
				const Imath::Box2i&, const image_view_t&,
				const warp_function_t&, bool, bool)> warp_bilinear_mirror = sse2::warp_bilinear_mirror;

std::function<void ( const Imath::Box2i&, const const_image_view_t&,
				const Imath::Box2i&, const image_view_t&,
				const warp_function_t&, bool, bool)> warp_bicubic = sse2::warp_bicubic;

// key mix
std::function<void ( const const_image_view_t&, const const_image_view_t&,
                       const const_channel_view_t&, const image_view_t&)> key_mix = sse2::key_mix;

// convolve
std::function<void ( const const_image_view_t&, const image_view_t&, const Imath::M33f&)> convolve33 = generic::convolve33;

// box blur
std::function<void ( const const_image_view_t&, const image_view_t&, 
					   const image_view_t&, float, float, int iters)> box_blur_rgba_ = sse2::box_blur_rgba;

std::function<void ( const const_channel_view_t&, const boost::gil::gray32f_view_t&,
					   const channel_view_t&, float, float, int)>  box_blur_channel_ = generic::box_blur_channel;

std::function<void ( const boost::gil::gray32fc_view_t&, const boost::gil::gray32f_view_t&,
					   const boost::gil::gray32f_view_t&, float, float, int)> box_blur_gray_ = generic::box_blur_gray;

// gaussian blur
std::function<void ( const const_image_view_t&, const image_view_t&, 
					   const image_view_t&, float, float)> gaussian_blur_rgba_ = sse2::gaussian_blur_rgba;

std::function<void( const const_rgb_image_view_t&, const rgb_image_view_t&,
					  const rgb_image_view_t&, float, float)> gaussian_blur_rgb_ = generic::gaussian_blur_rgb;

std::function<void ( const const_channel_view_t&, const boost::gil::gray32f_view_t&,
						  const channel_view_t&, float, float)>  gaussian_blur_channel_ = generic::gaussian_blur_channel;

std::function<void ( const boost::gil::gray32fc_view_t&, const boost::gil::gray32f_view_t&,
						  const boost::gil::gray32f_view_t&, float, float)> gaussian_blur_gray_ = generic::gaussian_blur_gray;

} // namespace
} // namespace
