// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ocio/gl_lut3d.hpp>

#include<algorithm>

#include<ramen/assert.hpp>

namespace ramen
{
namespace ocio
{

gl_lut3d_t::gl_lut3d_t( int lut_size, GLenum texture_unit) : gl::lut3d_t( lut_size, texture_unit)
{
	black_ = Imath::Color3f( 0, 0, 0);
}

void gl_lut3d_t::recreate( OCIO::ConstConfigRcPtr config, OCIO::DisplayViewTransformRcPtr transform, const std::string& fun_name)
{
    RAMEN_ASSERT( transform);
    OCIO::ConstProcessorRcPtr processor = config->getProcessor( transform);
	recreate( config, transform, processor, fun_name);
}

void gl_lut3d_t::recreate( OCIO::ConstConfigRcPtr config, OCIO::DisplayViewTransformRcPtr transform,
							OCIO::ConstProcessorRcPtr processor, const std::string& fun_name)
{
    OCIO::GpuShaderDescRcPtr desc = OCIO::GpuShaderDesc::CreateShaderDesc();
    desc->setLanguage( OCIO::GPU_LANGUAGE_GLSL_4_0);
    desc->setFunctionName( fun_name.c_str());

    // bake the whole transform into one 3D lut.
    OCIO::ConstGPUProcessorRcPtr gpu_processor = processor->getOptimizedLegacyGPUProcessor( OCIO::OPTIMIZATION_DEFAULT, lut_size());

    std::string cache_id = gpu_processor->getCacheID();

    if( cache_id == lut_cache_id_)
        return;

    lut_cache_id_ = cache_id;
    gpu_processor->extractGpuShaderInfo( desc);
    lut_fun_ = desc->getShaderText();
    sampler_name_.clear();

    if( desc->getNum3DTextures() > 0)
    {
        const char *texture_name = 0;
        const char *sampler_name = 0;
        unsigned edge_len = 0;
        OCIO::Interpolation interpolation = OCIO::INTERP_LINEAR;
        desc->get3DTexture( 0, texture_name, sampler_name, edge_len, interpolation);
        RAMEN_ASSERT( edge_len == lut_size());

        const float *values = 0;
        desc->get3DTextureValues( 0, values);
        std::copy( values, values + ( 3 * edge_len * edge_len * edge_len), reinterpret_cast<float*>( data()));
        sampler_name_ = sampler_name;
    }

    black_ = Imath::Color3f( 0, 0, 0);
    processor->getDefaultCPUProcessor()->applyRGB( &black_.x);
	update_gl_texture();
}

const Imath::Color3f& gl_lut3d_t::black() const { return black_;}

} // namespace
} // namespace
