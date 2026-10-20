// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/GL/program.hpp>

#include<ramen/assert.hpp>

#include<iostream>

namespace ramen
{
namespace gl
{

program_t::program_t( GLenum shader_type) : shader_type_( shader_type)
{
	shader_ = 0;
	vertex_shader_ = 0;
	program_ = 0;
}

program_t::~program_t()
{
	gl_delete_program( program_);
	gl_delete_shader( shader_);
	gl_delete_shader( vertex_shader_);
}

namespace
{

void print_shader_log( GLuint shader)
{
    GLint result;
    gl_functions()->glGetShaderiv( shader, GL_COMPILE_STATUS, &result);

    if( result == GL_FALSE)
    {
        char buffer[1025];
        GLsizei size = 0;
        gl_functions()->glGetShaderInfoLog( shader, 1024, &size, buffer);
        buffer[ size] = 0;
        std::cout << buffer << "\n";
    }
}

} // unnamed

void program_t::compile( const char *src0, const char *src1)
{
	RAMEN_ASSERT( shader_type_ == GL_FRAGMENT_SHADER);

	if( !program_)
	{
		program_ = gl_create_program();

		// the drawing functions provide the vertex shader.
		vertex_shader_ = gl_create_shader( GL_VERTEX_SHADER);
		const GLchar *vs_src = gl_vertex_shader_source();
		gl_shader_source( vertex_shader_, 1, &vs_src, 0);
		gl_compile_shader( vertex_shader_);
		print_shader_log( vertex_shader_);
		gl_attach_shader( program_, vertex_shader_);
		gl_bind_vertex_attributes( program_);
	}
	else
	{
		gl_detach_shader( program_, shader_);
		gl_delete_shader( shader_);
	}

	shader_ = gl_create_shader( shader_type_);

    const GLchar *shader_src[2];
    shader_src[0] = (GLchar *) src0;
    shader_src[1] = (GLchar *) src1;

	gl_shader_source( shader_, src1 ? 2 : 1, shader_src, 0);
    gl_compile_shader( shader_);
    print_shader_log( shader_);

    gl_attach_shader( program_, shader_);
    gl_link_program( program_);

    GLint result;
    gl_functions()->glGetProgramiv( program_, GL_LINK_STATUS, &result);
	check_gl_errors();

    if( result == GL_FALSE)
    {
        char buffer[1025];
        GLsizei size = 0;
        gl_functions()->glGetProgramInfoLog( program_, 1024, &size, buffer);
        buffer[ size] = 0;
        std::cout << buffer << "\n";
    }
}

void program_t::bind()
{
	RAMEN_ASSERT( program_);
	gl_use_program( program_);
}

void program_t::unbind() { gl_use_program( 0);}

void program_t::uniform_1i( const char *param, int v)
{
	RAMEN_ASSERT( program_);
    gl_uniform1i( gl_get_uniform_location( program_, param), v);
	check_gl_errors();
}

void program_t::uniform_1f( const char *param, float v)
{
	RAMEN_ASSERT( program_);
    gl_uniform1f( gl_get_uniform_location( program_, param), v);
	check_gl_errors();
}

void program_t::uniform_4f( const char *param, float *v)
{
	RAMEN_ASSERT( program_);
    gl_uniform4f( gl_get_uniform_location( program_, param), v[0], v[1], v[2], v[3]);
	check_gl_errors();
}

} // namespace
} // namespace
