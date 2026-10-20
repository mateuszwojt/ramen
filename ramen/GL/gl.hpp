// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_GL_HPP
#define	RAMEN_GL_HPP

#include<ramen/config.hpp>

#include<QOpenGLFunctions_4_1_Core>

// Ramen draws with an OpenGL 4.1 core profile context. These functions keep
// the old fixed function style interface ( immediate mode, matrix stacks,
// wide lines, evaluators) and implement it with buffers and shaders.

// fixed function enums, not defined by the core profile headers.
#ifndef GL_QUADS
    #define GL_QUADS 0x0007
#endif

#ifndef GL_POLYGON
    #define GL_POLYGON 0x0009
#endif

#ifndef GL_MODELVIEW
    #define GL_MODELVIEW 0x1700
#endif

#ifndef GL_PROJECTION
    #define GL_PROJECTION 0x1701
#endif

#ifndef GL_MAP1_VERTEX_3
    #define GL_MAP1_VERTEX_3 0x0D97
#endif

#ifndef GL_TEXTURE_ENV
    #define GL_TEXTURE_ENV 0x2300
#endif

#ifndef GL_TEXTURE_ENV_MODE
    #define GL_TEXTURE_ENV_MODE 0x2200
#endif

#ifndef GL_MODULATE
    #define GL_MODULATE 0x2100
#endif

#ifndef GL_CLAMP
    #define GL_CLAMP 0x2900
#endif

namespace ramen
{

/// Requests OpenGL 4.1 core profile contexts. Call it before creating the QApplication.
void gl_set_default_format();

/// Returns the OpenGL functions of the current context.
QOpenGLFunctions_4_1_Core *gl_functions();

/// Returns true if the current context supports OpenGL 4.1 core.
bool gl_context_supported();

void clear_gl_errors();
void check_gl_errors();

void gl_enable( GLenum cap);
void gl_disable( GLenum cap);
void gl_blend_func( GLenum sfactor, GLenum dfactor);

// programs
GLuint gl_create_program();
void gl_delete_program( GLuint program);
void gl_link_program( GLuint program);
void gl_use_program( GLuint program);
GLint gl_get_uniform_location( GLuint program, const GLchar *name);
void gl_uniform1i( GLint location, GLint v0);
void gl_uniform1f( GLint location, GLfloat v0);
void gl_uniform4f( GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

/// Binds the vertex attributes used by the drawing functions. Call it before linking.
void gl_bind_vertex_attributes( GLuint program);

/// Vertex shader that programs used with the drawing functions can link with.
const char *gl_vertex_shader_source();

// shaders
GLuint gl_create_shader( GLenum shader_type);
void gl_delete_shader( GLuint shader);
void gl_shader_source( GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
void gl_compile_shader( GLuint shader);
void gl_attach_shader( GLuint program, GLuint shader);
void gl_detach_shader( GLuint program, GLuint shader);

// textures
GLuint gl_gen_texture();
void gl_gen_textures( GLsizei num, GLuint *tex);
void gl_delete_texture( GLuint *tex);
void gl_delete_textures( int num, GLuint *tex);
void gl_bind_texture( GLenum target, GLuint texture);
void gl_active_texture( GLenum texture);
void gl_tex_parameteri( GLenum target, GLenum pname, GLint param);
void gl_tex_envf( GLenum target, GLenum pname, GLfloat param);

void gl_tex_image2d( GLenum target, GLint level, GLint internal_format,
				 	GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data);

void gl_tex_subimage2d(	GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height,
						GLenum format, GLenum type, const GLvoid *data);

void gl_tex_image3d( GLenum target, GLint level, GLint internal_format, GLsizei width,
					 GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *data);

void gl_tex_subimage3d(	GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset,
						GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *data);

void gl_get_tex_image( GLenum target, GLint level, GLenum format, GLenum type, GLvoid *img);

// pixels
void gl_pixel_storei( GLenum pname, GLint param);

// transform
void gl_matrix_mode( GLenum mode);

void gl_push_matrix();
void gl_pop_matrix();

void gl_load_identity();
void gl_translatef( GLfloat x, GLfloat y, GLfloat z = 0);
void gl_scalef( GLfloat x, GLfloat y, GLfloat z = 1);

/// m is a column major 4x4 matrix, like the one glLoadMatrixf takes.
void gl_load_matrixf( const GLfloat *m);
void gl_mult_matrixf( const GLfloat *m);

void gl_ortho( double left, double right, double bottom, double top, double near_val, double far_val);

void gl_viewport( GLint x, GLint y, GLsizei width, GLsizei height);

// draw
void gl_clear( GLbitfield mask);
void gl_clear_color( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

void gl_begin( GLenum mode);
void gl_end();
bool inside_begin_block();

void gl_vertex2i( GLint x, GLint y);
void gl_vertex2f( GLfloat x, GLfloat y);

void gl_color3ub( GLubyte r, GLubyte g, GLubyte b);
void gl_color3f( GLfloat r, GLfloat g, GLfloat b);
void gl_color4f( GLfloat r, GLfloat g, GLfloat b, GLfloat a);

void gl_tex_coord2f( GLfloat s, GLfloat t);

void gl_point_size( GLfloat size);
void gl_line_width( GLfloat width);

// map & eval
void gl_map1f( GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
void gl_eval_coord1f( GLfloat u);

// more
void gl_flush();
void gl_finish();

// util
void gl_textured_quad( int x0, int y0, int x1, int y1, bool flip_tex_v = false);

} // ramen

#endif
