// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ocio/gl_display_manager.hpp>

#include<cmath>

#include<algorithm>

#include<ramen/assert.hpp>

namespace ramen
{
namespace ocio
{
namespace
{
static const char *ocio_display_shader_source =
{
"uniform sampler2D tex;"
"uniform float exposure;"
"uniform float gamma;"
"uniform vec4 channel_mask;"
"uniform float alpha;"
"void main( void)"
"{"
	"vec4 col = texture2D( tex, gl_TexCoord[0].st);"
	"vec4 acol = vec4( col.a, col.a, col.a, col.a);"
	"vec4 expv = vec4( exposure, exposure, exposure, 1);"
	"col *= expv;"
	"col = apply_lut( col) * channel_mask;"
    "vec4 gammav = vec4( gamma, gamma, gamma, 1);"
    "col = pow( col, gammav);"
	"col = (col * ( 1.0 - alpha)) + ( alpha * acol);"
	"gl_FragColor = max( col, vec4( 0, 0, 0, 0));"
"}"
};

} // unnamed

gl_display_manager_t::gl_display_manager_t( int lut_size, GLenum texture_num)
{
	lut_.reset( new gl_lut3d_t( lut_size, texture_num));
	init();
}

gl_display_manager_t::gl_display_manager_t( std::shared_ptr<gl_lut3d_t> lut) : lut_( lut)
{
	init();
}

void gl_display_manager_t::init()
{
	exposure_ = 0.0f;
	gamma_ = 1.0f;
	view_channels_ = view_rgb_channels;
	get_ctx_fun_ = &gl_display_manager_t::default_get_ctx;
}

void gl_display_manager_t::set_context_callback( const get_context_callback_t& f)
{
	RAMEN_ASSERT( f);
	get_ctx_fun_ = f;
}

void gl_display_manager_t::set_display_transform( OCIO::ConstConfigRcPtr config, OCIO::DisplayViewTransformRcPtr transform)
{
	OCIO::ConstContextRcPtr context = get_local_context();
    processor_ = config->getProcessor( context, transform, OCIO::TRANSFORM_DIR_FORWARD);
    lut_->recreate( config, transform, processor_, "apply_lut");
	// without a version directive, strict drivers compile the shaders as GLSL 1.10.
	std::string lut_fun( "#version 120\n");
	lut_fun += lut_->lookup_function();
	program_.compile( lut_fun.c_str(), ocio_display_shader_source);
}

Imath::Color3f gl_display_manager_t::transform( const Imath::Color3f& c) const
{
	RAMEN_ASSERT( processor_);
	Imath::Color3f result( c);
	processor_->getDefaultCPUProcessor()->applyRGB( ( float*) &( result.x));
	return result;
}

Imath::Color4f gl_display_manager_t::transform( const Imath::Color4f& c) const
{
	RAMEN_ASSERT( processor_);
	Imath::Color4f result( c);
	processor_->getDefaultCPUProcessor()->applyRGBA( ( float*) &( result.r));
	return result;
}

void gl_display_manager_t::activate()
{
	RAMEN_ASSERT( processor_);

	lut_->bind();
	program_.bind();

	// TODO: don't hardcode the texture unit numbers
	program_.uniform_1i( "tex", 0);
	if( !lut_->sampler_name().empty())
		program_.uniform_1i( lut_->sampler_name().c_str(), 1);
	program_.uniform_1f( "exposure", std::pow( 2.0, ( double) exposure()));
	program_.uniform_1f( "gamma", 1.0 / std::max( 1e-4, ( double) gamma()));

	Imath::Color4f cmask( 1, 1, 1, 1);
	float alpha = 0;

	switch( view_channels_)
	{
	case view_red_channel:
		cmask = Imath::Color4f( 1, 0, 0, 1);
	break;

	case view_green_channel:
		cmask = Imath::Color4f( 0, 1, 0, 1);
	break;

	case view_blue_channel:
		cmask = Imath::Color4f( 0, 0, 1, 1);
	break;

	case view_alpha_channel:
		alpha = 1;
	break;
	}

	program_.uniform_4f( "channel_mask", ( float *) &(cmask.r));
	program_.uniform_1f( "alpha", alpha);
}

void gl_display_manager_t::deactivate()
{
	RAMEN_ASSERT( processor_);

	lut_->unbind();
	program_.unbind();
}

std::pair<std::string, std::string> gl_display_manager_t::default_get_ctx( int i)
{
	return std::make_pair( std::string(), std::string());
}

OCIO::ConstContextRcPtr gl_display_manager_t::get_local_context()
{
	OCIO::ConstConfigRcPtr config = OCIO::GetCurrentConfig();
	OCIO::ConstContextRcPtr context = config->getCurrentContext();
	
	if( !get_ctx_fun_)
		return context;
	
    OCIO::ContextRcPtr mutable_context;
	
	std::pair<std::string, std::string> ctx_pair;

	ctx_pair = get_ctx_fun_( 0);
	
    if( !ctx_pair.first.empty())
    {
        if( !mutable_context)
			mutable_context = context->createEditableCopy();

        mutable_context->setStringVar( ctx_pair.first.c_str(), ctx_pair.second.c_str());		
    }
	
	ctx_pair = get_ctx_fun_( 1);
	
    if( !ctx_pair.first.empty())
	{
		if( !mutable_context)
			mutable_context = context->createEditableCopy();
		
        mutable_context->setStringVar( ctx_pair.first.c_str(), ctx_pair.second.c_str());		
    }

	ctx_pair = get_ctx_fun_( 2);
	
    if( !ctx_pair.first.empty())
    {
		if( !mutable_context)
			mutable_context = context->createEditableCopy();
		
        mutable_context->setStringVar( ctx_pair.first.c_str(), ctx_pair.second.c_str());		
    }
	
	ctx_pair = get_ctx_fun_( 3);
	
    if( !ctx_pair.first.empty())		
    {
        if(!mutable_context)
			mutable_context = context->createEditableCopy();

        mutable_context->setStringVar( ctx_pair.first.c_str(), ctx_pair.second.c_str());
    }
	
    if( mutable_context)
		context = mutable_context;
	
	return context;
}

} // ocio
} // ramen
