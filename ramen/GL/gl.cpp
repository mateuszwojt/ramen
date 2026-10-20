// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/GL/gl.hpp>

#include<cmath>
#include<cstring>
#include<map>
#include<memory>
#include<vector>

#ifndef NDEBUG
    #include<iostream>
#endif

#include<QOpenGLContext>
#include<QOpenGLVersionFunctionsFactory>
#include<QSurfaceFormat>

#include<ramen/assert.hpp>

namespace ramen
{
namespace
{

// column major 4x4 matrix, like OpenGL.
struct matrix_t
{
    matrix_t() { set_identity();}

    void set_identity()
    {
        std::memset( m, 0, sizeof( m));
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }

    // this = this * other
    void multiply( const float *other)
    {
        float r[16];

        for( int c = 0; c < 4; ++c)
        {
            for( int row = 0; row < 4; ++row)
            {
                r[c * 4 + row] = m[0 * 4 + row] * other[c * 4 + 0] +
                                 m[1 * 4 + row] * other[c * 4 + 1] +
                                 m[2 * 4 + row] * other[c * 4 + 2] +
                                 m[3 * 4 + row] * other[c * 4 + 3];
            }
        }

        std::memcpy( m, r, sizeof( m));
    }

    void transform( float x, float y, float out[4]) const
    {
        for( int row = 0; row < 4; ++row)
            out[row] = m[row] * x + m[4 + row] * y + m[12 + row];
    }

    float m[16];
};

matrix_t operator*( const matrix_t& a, const matrix_t& b)
{
    matrix_t r( a);
    r.multiply( b.m);
    return r;
}

struct vertex_t
{
    float x, y;
    float u, v;
    float r, g, b, a;
};

const char *g_vertex_shader_src =
    "#version 410 core\n"
    "layout( location = 0) in vec2 ramen_position;\n"
    "layout( location = 1) in vec2 ramen_texcoord;\n"
    "layout( location = 2) in vec4 ramen_color;\n"
    "uniform mat4 ramen_mvp;\n"
    "uniform float ramen_point_size;\n"
    "out vec2 ramen_uv;\n"
    "out vec4 ramen_vcolor;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = ramen_mvp * vec4( ramen_position, 0.0, 1.0);\n"
    "    gl_PointSize = ramen_point_size;\n"
    "    ramen_uv = ramen_texcoord;\n"
    "    ramen_vcolor = ramen_color;\n"
    "}\n";

// texture modes: 0 no texture, 1 modulate, 2 replace.
const char *g_fragment_shader_src =
    "#version 410 core\n"
    "in vec2 ramen_uv;\n"
    "in vec4 ramen_vcolor;\n"
    "uniform int ramen_texture_mode;\n"
    "uniform sampler2D ramen_texture;\n"
    "uniform int ramen_line_aa;\n"
    "uniform float ramen_line_half_width;\n"
    "out vec4 ramen_frag_color;\n"
    "void main()\n"
    "{\n"
    "    vec4 c = ramen_vcolor;\n"
    "    if( ramen_texture_mode == 1)\n"
    "        c *= texture( ramen_texture, ramen_uv);\n"
    "    else if( ramen_texture_mode == 2)\n"
    "        c = texture( ramen_texture, ramen_uv);\n"
    "    if( ramen_line_aa == 1)\n"
    "        c.a *= clamp( ramen_line_half_width + 0.5 - abs( ramen_uv.y), 0.0, 1.0);\n"
    "    ramen_frag_color = c;\n"
    "}\n";

// GL objects, they are not shared between contexts.
struct context_state_t
{
    QOpenGLFunctions_4_1_Core *f = 0;
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint program = 0;
};

// state of the fixed function emulation.
struct state_t
{
    std::map<QOpenGLContext*, std::unique_ptr<context_state_t> > contexts;

    bool inside_begin = false;
    GLenum mode = 0;
    std::vector<vertex_t> vertices;

    float color[4] = { 1, 1, 1, 1};
    float uv[2] = { 0, 0};

    float line_width = 1.0f;
    float point_size = 1.0f;

    bool texture_2d = false;
    bool line_smooth = false;
    GLfloat texture_env_mode = GL_MODULATE;

    GLuint user_program = 0;

    GLenum matrix_mode = GL_MODELVIEW;
    std::vector<matrix_t> modelview = std::vector<matrix_t>( 1);
    std::vector<matrix_t> projection = std::vector<matrix_t>( 1);

    // evaluator
    std::vector<float> map_points;
    int map_order = 0;
    int map_stride = 3;
    float map_u1 = 0.0f;
    float map_u2 = 1.0f;

    std::vector<matrix_t>& current_stack()
    {
        return matrix_mode == GL_PROJECTION ? projection : modelview;
    }

    matrix_t& current_matrix() { return current_stack().back();}
};

state_t& state()
{
    static state_t s;
    return s;
}

GLuint compile_shader( QOpenGLFunctions_4_1_Core *f, GLenum type, const char *src)
{
    GLuint shader = f->glCreateShader( type);
    f->glShaderSource( shader, 1, &src, 0);
    f->glCompileShader( shader);

    #ifndef NDEBUG
        GLint ok;
        f->glGetShaderiv( shader, GL_COMPILE_STATUS, &ok);

        if( !ok)
        {
            char log[1024];
            f->glGetShaderInfoLog( shader, sizeof( log), 0, log);
            std::cerr << "OpenGL shader error: " << log << "\n";
        }
    #endif

    return shader;
}

context_state_t& context_state()
{
    QOpenGLContext *ctx = QOpenGLContext::currentContext();
    RAMEN_ASSERT( ctx && "No current OpenGL context");

    std::unique_ptr<context_state_t>& cs( state().contexts[ctx]);

    if( !cs)
    {
        cs.reset( new context_state_t());
        cs->f = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_1_Core>( ctx);
        RAMEN_ASSERT( cs->f && "OpenGL 4.1 core profile not available");
        cs->f->initializeOpenGLFunctions();

        QObject::connect( ctx, &QOpenGLContext::aboutToBeDestroyed, [ctx]() { state().contexts.erase( ctx);});

        QOpenGLFunctions_4_1_Core *f = cs->f;

        f->glGenVertexArrays( 1, &cs->vao);
        f->glBindVertexArray( cs->vao);

        f->glGenBuffers( 1, &cs->vbo);
        f->glBindBuffer( GL_ARRAY_BUFFER, cs->vbo);

        f->glEnableVertexAttribArray( 0);
        f->glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof( vertex_t), reinterpret_cast<void*>( offsetof( vertex_t, x)));
        f->glEnableVertexAttribArray( 1);
        f->glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( vertex_t), reinterpret_cast<void*>( offsetof( vertex_t, u)));
        f->glEnableVertexAttribArray( 2);
        f->glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, sizeof( vertex_t), reinterpret_cast<void*>( offsetof( vertex_t, r)));

        GLuint vs = compile_shader( f, GL_VERTEX_SHADER, g_vertex_shader_src);
        GLuint fs = compile_shader( f, GL_FRAGMENT_SHADER, g_fragment_shader_src);
        cs->program = f->glCreateProgram();
        f->glAttachShader( cs->program, vs);
        f->glAttachShader( cs->program, fs);
        f->glLinkProgram( cs->program);
        f->glDeleteShader( vs);
        f->glDeleteShader( fs);

        f->glEnable( GL_PROGRAM_POINT_SIZE);
    }

    return *cs;
}

void set_uniform_matrix( QOpenGLFunctions_4_1_Core *f, GLuint program, const matrix_t& m)
{
    GLint loc = f->glGetUniformLocation( program, "ramen_mvp");

    if( loc != -1)
        f->glUniformMatrix4fv( loc, 1, GL_FALSE, m.m);
}

bool is_line_mode( GLenum mode)
{
    return mode == GL_LINES || mode == GL_LINE_STRIP || mode == GL_LINE_LOOP;
}

// expands lines wider than one pixel into triangles, in normalized device coordinates.
std::vector<vertex_t> expand_wide_lines( const std::vector<vertex_t>& verts, GLenum mode, const matrix_t& mvp,
                                         const GLint viewport[4], float width, bool antialias)
{
    std::vector<vertex_t> segments;

    if( mode == GL_LINES)
        segments.assign( verts.begin(), verts.end() - ( verts.size() % 2));
    else
    {
        for( std::size_t i = 0; i + 1 < verts.size(); ++i)
        {
            segments.push_back( verts[i]);
            segments.push_back( verts[i + 1]);
        }

        if( mode == GL_LINE_LOOP && verts.size() > 2)
        {
            segments.push_back( verts.back());
            segments.push_back( verts.front());
        }
    }

    // to window coordinates.
    auto to_window = [&]( const vertex_t& v, float& wx, float& wy)
    {
        float c[4];
        mvp.transform( v.x, v.y, c);
        float w = c[3] != 0.0f ? c[3] : 1.0f;
        wx = ( c[0] / w * 0.5f + 0.5f) * viewport[2] + viewport[0];
        wy = ( c[1] / w * 0.5f + 0.5f) * viewport[3] + viewport[1];
    };

    auto to_ndc = [&]( const vertex_t& src, float wx, float wy)
    {
        vertex_t v( src);
        v.x = ( wx - viewport[0]) / viewport[2] * 2.0f - 1.0f;
        v.y = ( wy - viewport[1]) / viewport[3] * 2.0f - 1.0f;
        return v;
    };

    std::vector<vertex_t> result;

    // anti-aliased lines have a one pixel wide border where they fade out.
    const float half = width * 0.5f + ( antialias ? 1.0f : 0.0f);

    for( std::size_t i = 0; i + 1 < segments.size(); i += 2)
    {
        float ax, ay, bx, by;
        to_window( segments[i], ax, ay);
        to_window( segments[i + 1], bx, by);

        float dx = bx - ax;
        float dy = by - ay;
        float len = std::sqrt( dx * dx + dy * dy);

        if( len == 0.0f)
            continue;

        float nx = -dy / len * half;
        float ny =  dx / len * half;

        vertex_t a0( to_ndc( segments[i], ax + nx, ay + ny));
        vertex_t a1( to_ndc( segments[i], ax - nx, ay - ny));
        vertex_t b0( to_ndc( segments[i + 1], bx + nx, by + ny));
        vertex_t b1( to_ndc( segments[i + 1], bx - nx, by - ny));

        // distance to the line center, in pixels.
        a0.v = b0.v = half;
        a1.v = b1.v = -half;

        result.push_back( a0); result.push_back( a1); result.push_back( b0);
        result.push_back( b0); result.push_back( a1); result.push_back( b1);
    }

    return result;
}

} // unnamed

void gl_set_default_format()
{
    QSurfaceFormat format( QSurfaceFormat::defaultFormat());
    format.setVersion( 4, 1);
    format.setProfile( QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat( format);
}

QOpenGLFunctions_4_1_Core *gl_functions() { return context_state().f;}

bool gl_context_supported()
{
    QOpenGLContext *ctx = QOpenGLContext::currentContext();

    if( !ctx)
        return false;

    QSurfaceFormat format( ctx->format());

    if( format.majorVersion() < 4 || ( format.majorVersion() == 4 && format.minorVersion() < 1))
        return false;

    return QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_1_Core>( ctx) != 0;
}

void clear_gl_errors()
{
	while( gl_functions()->glGetError() != GL_NO_ERROR)
		;
}

void check_gl_errors()
{
	#ifndef NDEBUG
		GLenum err = gl_functions()->glGetError();

		if( err != GL_NO_ERROR)
            std::cerr << "OpenGL error: 0x" << std::hex << err << std::dec << "\n";
	#endif
}

void gl_enable( GLenum cap)
{
    switch( cap)
    {
        case GL_TEXTURE_2D:
            state().texture_2d = true;
        break;

        case GL_LINE_SMOOTH:
            state().line_smooth = true;
        break;

        // fixed function state, nothing to do.
        case GL_TEXTURE_3D:
        case GL_MAP1_VERTEX_3:
        break;

        default:
            gl_functions()->glEnable( cap);
            check_gl_errors();
    }
}

void gl_disable( GLenum cap)
{
    switch( cap)
    {
        case GL_TEXTURE_2D:
            state().texture_2d = false;
        break;

        case GL_LINE_SMOOTH:
            state().line_smooth = false;
        break;

        case GL_TEXTURE_3D:
        case GL_MAP1_VERTEX_3:
        break;

        default:
            gl_functions()->glDisable( cap);
            check_gl_errors();
    }
}

void gl_blend_func( GLenum sfactor, GLenum dfactor)
{
	gl_functions()->glBlendFunc( sfactor, dfactor);
	check_gl_errors();
}

// programs
GLuint gl_create_program()
{
	GLuint result = gl_functions()->glCreateProgram();
	check_gl_errors();
	return result;
}

void gl_delete_program( GLuint program)
{
	if( program != 0)
	{
		gl_functions()->glDeleteProgram( program);
		check_gl_errors();
	}
}

void gl_link_program( GLuint program)
{
	gl_functions()->glLinkProgram( program);
	check_gl_errors();
}

void gl_use_program( GLuint program)
{
    state().user_program = program;

    // the drawing functions bind their own program when program is 0.
    if( program)
    {
        gl_functions()->glUseProgram( program);
        check_gl_errors();
    }
}

GLint gl_get_uniform_location( GLuint program, const GLchar *name)
{
	GLint result = gl_functions()->glGetUniformLocation( program, name);
	check_gl_errors();
	return result;
}

void gl_uniform1i( GLint location, GLint v0)
{
	gl_functions()->glUniform1i( location, v0);
	check_gl_errors();
}

void gl_uniform1f( GLint location, GLfloat v0)
{
	gl_functions()->glUniform1f( location, v0);
	check_gl_errors();
}

void gl_uniform4f( GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	gl_functions()->glUniform4f( location, v0, v1, v2, v3);
	check_gl_errors();
}

void gl_bind_vertex_attributes( GLuint program)
{
    QOpenGLFunctions_4_1_Core *f = gl_functions();
    f->glBindAttribLocation( program, 0, "ramen_position");
    f->glBindAttribLocation( program, 1, "ramen_texcoord");
    f->glBindAttribLocation( program, 2, "ramen_color");
}

const char *gl_vertex_shader_source() { return g_vertex_shader_src;}

// shaders
GLuint gl_create_shader( GLenum shader_type)
{
	GLuint result = gl_functions()->glCreateShader( shader_type);
	check_gl_errors();
	return result;
}

void gl_delete_shader( GLuint shader)
{
	if( shader)
	{
		gl_functions()->glDeleteShader( shader);
		check_gl_errors();
	}
}

void gl_shader_source( GLuint shader, GLsizei count, const GLchar **string, const GLint *length)
{
	gl_functions()->glShaderSource( shader, count, string, length);
	check_gl_errors();
}

void gl_compile_shader( GLuint shader)
{
	gl_functions()->glCompileShader( shader);
	check_gl_errors();
}

void gl_attach_shader( GLuint program, GLuint shader)
{
	gl_functions()->glAttachShader( program, shader);
	check_gl_errors();
}

void gl_detach_shader( GLuint program, GLuint shader)
{
	gl_functions()->glDetachShader( program, shader);
	check_gl_errors();
}

// textures
GLuint gl_gen_texture()
{
	GLuint tex = 0;
	gl_gen_textures( 1, &tex);
	return tex;
}

void gl_gen_textures( GLsizei num, GLuint *tex)
{
	gl_functions()->glGenTextures( num, tex);
	check_gl_errors();
}

void gl_delete_texture( GLuint *tex)
{
	if( *tex)
		gl_delete_textures( 1, tex);
}

void gl_delete_textures( GLsizei num, GLuint *tex)
{
	gl_functions()->glDeleteTextures( num, tex);
	check_gl_errors();
}

void gl_bind_texture( GLenum target, GLuint texture)
{
	RAMEN_ASSERT( !inside_begin_block());
	gl_functions()->glBindTexture( target, texture);
	check_gl_errors();
}

void gl_active_texture( GLenum texture)
{
	gl_functions()->glActiveTexture( texture);
	check_gl_errors();
}

void gl_tex_parameteri( GLenum target, GLenum pname, GLint param)
{
    // GL_CLAMP is not in the core profile.
    if( param == GL_CLAMP)
        param = GL_CLAMP_TO_EDGE;

	gl_functions()->glTexParameteri( target, pname, param);
	check_gl_errors();
}

void gl_tex_envf( GLenum target, GLenum pname, GLfloat param)
{
    if( target == GL_TEXTURE_ENV && pname == GL_TEXTURE_ENV_MODE)
        state().texture_env_mode = param;
}

void gl_tex_image2d( GLenum target, GLint level, GLint internal_format,
				 	GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data)
{
	gl_functions()->glTexImage2D( target, level, internal_format, width, height, border, format, type, data);
	check_gl_errors();
}

void gl_tex_subimage2d(	GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height,
						GLenum format, GLenum type, const GLvoid *data)
{
	gl_functions()->glTexSubImage2D( target, level, xoffset, yoffset, width, height, format, type, data);
	check_gl_errors();
}

void gl_tex_image3d( GLenum target, GLint level, GLint internal_format, GLsizei width,
					 GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *data)
{
	gl_functions()->glTexImage3D( target, level, internal_format, width, height, depth, border, format, type, data);
	check_gl_errors();
}

void gl_tex_subimage3d(	GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset,
						GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *data)
{
	gl_functions()->glTexSubImage3D( target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, data);
	check_gl_errors();
}

void gl_get_tex_image( GLenum target, GLint level, GLenum format, GLenum type, GLvoid *img)
{
	gl_functions()->glGetTexImage( target, level, format, type, img);
	check_gl_errors();
}

// pixels
void gl_pixel_storei( GLenum pname, GLint param)
{
	gl_functions()->glPixelStorei( pname, param);
	check_gl_errors();
}

// transform
void gl_matrix_mode( GLenum mode)
{
    RAMEN_ASSERT( mode == GL_MODELVIEW || mode == GL_PROJECTION);
    state().matrix_mode = mode;
}

void gl_push_matrix()
{
    std::vector<matrix_t>& stack( state().current_stack());
    stack.push_back( stack.back());
}

void gl_pop_matrix()
{
    std::vector<matrix_t>& stack( state().current_stack());
    RAMEN_ASSERT( stack.size() > 1 && "Matrix stack underflow");

    if( stack.size() > 1)
        stack.pop_back();
}

void gl_load_identity() { state().current_matrix().set_identity();}

void gl_translatef( GLfloat x, GLfloat y, GLfloat z)
{
    matrix_t t;
    t.m[12] = x;
    t.m[13] = y;
    t.m[14] = z;
    state().current_matrix().multiply( t.m);
}

void gl_scalef( GLfloat x, GLfloat y, GLfloat z)
{
    matrix_t s;
    s.m[0] = x;
    s.m[5] = y;
    s.m[10] = z;
    state().current_matrix().multiply( s.m);
}

void gl_load_matrixf( const GLfloat *m) { std::memcpy( state().current_matrix().m, m, sizeof( GLfloat) * 16);}
void gl_mult_matrixf( const GLfloat *m) { state().current_matrix().multiply( m);}

void gl_ortho( double left, double right, double bottom, double top, double near_val, double far_val)
{
    matrix_t o;
    o.m[0] = 2.0 / ( right - left);
    o.m[5] = 2.0 / ( top - bottom);
    o.m[10] = -2.0 / ( far_val - near_val);
    o.m[12] = -( right + left) / ( right - left);
    o.m[13] = -( top + bottom) / ( top - bottom);
    o.m[14] = -( far_val + near_val) / ( far_val - near_val);
    state().current_matrix().multiply( o.m);
}

void gl_viewport( GLint x, GLint y, GLsizei width, GLsizei height)
{
	gl_functions()->glViewport( x, y, width, height);
	check_gl_errors();
}

// draw
void gl_clear( GLbitfield mask)
{
	gl_functions()->glClear( mask);
	check_gl_errors();
}

void gl_clear_color( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	gl_functions()->glClearColor( red, green, blue, alpha);
	check_gl_errors();
}

void gl_begin( GLenum mode)
{
	RAMEN_ASSERT( !state().inside_begin);
	state().inside_begin = true;
	state().mode = mode;
	state().vertices.clear();
}

void gl_end()
{
    state_t& s( state());
	RAMEN_ASSERT( s.inside_begin);
	s.inside_begin = false;

    if( s.vertices.empty())
        return;

    context_state_t& cs( context_state());
    QOpenGLFunctions_4_1_Core *f = cs.f;

    matrix_t mvp( s.projection.back() * s.modelview.back());

    GLenum prim = s.mode;
    std::vector<vertex_t> verts;
    bool antialiased_lines = false;

    if( s.mode == GL_QUADS)
    {
        prim = GL_TRIANGLES;

        for( std::size_t i = 0; i + 3 < s.vertices.size(); i += 4)
        {
            verts.push_back( s.vertices[i]);
            verts.push_back( s.vertices[i + 1]);
            verts.push_back( s.vertices[i + 2]);
            verts.push_back( s.vertices[i]);
            verts.push_back( s.vertices[i + 2]);
            verts.push_back( s.vertices[i + 3]);
        }
    }
    else if( s.mode == GL_POLYGON)
    {
        prim = GL_TRIANGLE_FAN;
        verts.swap( s.vertices);
    }
    else if( is_line_mode( s.mode) && ( s.line_width > 1.0f || s.line_smooth))
    {
        // the core profile only has aliased, one pixel wide lines.
        GLint viewport[4];
        f->glGetIntegerv( GL_VIEWPORT, viewport);
        verts = expand_wide_lines( s.vertices, s.mode, mvp, viewport, s.line_width, s.line_smooth);
        prim = GL_TRIANGLES;
        mvp.set_identity();
        antialiased_lines = s.line_smooth;
    }
    else
        verts.swap( s.vertices);

    s.vertices.clear();

    if( verts.empty())
        return;

    f->glBindVertexArray( cs.vao);
    f->glBindBuffer( GL_ARRAY_BUFFER, cs.vbo);
    f->glBufferData( GL_ARRAY_BUFFER, verts.size() * sizeof( vertex_t), verts.data(), GL_STREAM_DRAW);

    GLuint program = s.user_program ? s.user_program : cs.program;
    f->glUseProgram( program);
    set_uniform_matrix( f, program, mvp);

    GLint loc = f->glGetUniformLocation( program, "ramen_point_size");

    if( loc != -1)
        f->glUniform1f( loc, s.point_size);

    if( program == cs.program)
    {
        int texture_mode = 0;

        if( s.texture_2d)
            texture_mode = s.texture_env_mode == GL_REPLACE ? 2 : 1;

        f->glUniform1i( f->glGetUniformLocation( program, "ramen_texture_mode"), antialiased_lines ? 0 : texture_mode);
        f->glUniform1i( f->glGetUniformLocation( program, "ramen_texture"), 0);
        f->glUniform1i( f->glGetUniformLocation( program, "ramen_line_aa"), antialiased_lines ? 1 : 0);
        f->glUniform1f( f->glGetUniformLocation( program, "ramen_line_half_width"), s.line_width * 0.5f);
    }

    f->glDrawArrays( prim, 0, static_cast<GLsizei>( verts.size()));
	check_gl_errors();
}

bool inside_begin_block() { return state().inside_begin;}

void gl_vertex2i( GLint x, GLint y) { gl_vertex2f( x, y);}

void gl_vertex2f( GLfloat x, GLfloat y)
{
    const state_t& s( state());
    vertex_t v = { x, y, s.uv[0], s.uv[1], s.color[0], s.color[1], s.color[2], s.color[3]};
    state().vertices.push_back( v);
}

void gl_color3ub( GLubyte r, GLubyte g, GLubyte b) { gl_color4f( r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);}
void gl_color3f( GLfloat r, GLfloat g, GLfloat b) { gl_color4f( r, g, b, 1.0f);}

void gl_color4f( GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    float *c = state().color;
    c[0] = r; c[1] = g; c[2] = b; c[3] = a;
}

void gl_tex_coord2f( GLfloat s, GLfloat t)
{
    state().uv[0] = s;
    state().uv[1] = t;
}

void gl_point_size( GLfloat size) { state().point_size = size;}
void gl_line_width( GLfloat width) { state().line_width = width;}

// map & eval
void gl_map1f( GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points)
{
    RAMEN_ASSERT( target == GL_MAP1_VERTEX_3);

    state_t& s( state());
    s.map_u1 = u1;
    s.map_u2 = u2;
    s.map_order = order;
    s.map_stride = stride;
    s.map_points.assign( points, points + stride * order);
}

void gl_eval_coord1f( GLfloat u)
{
    const state_t& s( state());

    if( s.map_order == 0)
        return;

    float t = ( u - s.map_u1) / ( s.map_u2 - s.map_u1);

    // de Casteljau
    std::vector<float> x( s.map_order), y( s.map_order);

    for( int i = 0; i < s.map_order; ++i)
    {
        x[i] = s.map_points[i * s.map_stride];
        y[i] = s.map_points[i * s.map_stride + 1];
    }

    for( int k = 1; k < s.map_order; ++k)
    {
        for( int i = 0; i < s.map_order - k; ++i)
        {
            x[i] = x[i] * ( 1.0f - t) + x[i + 1] * t;
            y[i] = y[i] * ( 1.0f - t) + y[i + 1] * t;
        }
    }

    gl_vertex2f( x[0], y[0]);
}

// more
void gl_flush()
{
	gl_functions()->glFlush();
	check_gl_errors();
}

void gl_finish()
{
	gl_functions()->glFinish();
	check_gl_errors();
}

// util
void gl_textured_quad( int x0, int y0, int x1, int y1, bool flip_tex_v)
{
	gl_begin( GL_QUADS);

		if( flip_tex_v)
		{
			gl_tex_coord2f( 0, 0);
			gl_vertex2i( x0, y0);

			gl_tex_coord2f( 1, 0);
			gl_vertex2i( x1, y0);

			gl_tex_coord2f( 1, 1);
			gl_vertex2i( x1, y1);

			gl_tex_coord2f( 0, 1);
			gl_vertex2i( x0, y1);
		}
		else
		{
			gl_tex_coord2f( 0, 1);
			gl_vertex2i( x0, y0);

			gl_tex_coord2f( 1, 1);
			gl_vertex2i( x1, y0);

			gl_tex_coord2f( 1, 0);
			gl_vertex2i( x1, y1);

			gl_tex_coord2f( 0, 0);
			gl_vertex2i( x0, y1);
		}

	gl_end();
}

} // ramen
