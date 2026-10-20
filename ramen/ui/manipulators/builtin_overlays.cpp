// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

// Viewer overlays of the builtin nodes.

#include<ramen/ui/manipulators/overlays.hpp>

#include<ramen/assert.hpp>

#include<ramen/app/composition.hpp>
#include<ramen/app/palette.hpp>

#include<ramen/params/float_param.hpp>
#include<ramen/params/float2_param.hpp>
#include<ramen/params/param_set.hpp>
#include<ramen/params/transform2_param.hpp>

#include<ramen/nodes/image/distort/twirl_node.hpp>
#include<ramen/nodes/image/generate/gradient_node.hpp>
#include<ramen/nodes/image/generate/rgradient_node.hpp>
#include<ramen/nodes/image/roto/roto_node.hpp>
#include<ramen/nodes/image/transform/move2d_node.hpp>

#include<ramen/GL/Imath_gl.hpp>

#include<ramen/ui/manipulators/circle_manipulator.hpp>
#include<ramen/ui/manipulators/draw.hpp>
#include<ramen/ui/manipulators/pick.hpp>
#include<ramen/ui/manipulators/point2_manipulator.hpp>
#include<ramen/ui/roto/roto_overlay.hpp>
#include<ramen/ui/viewer/viewer_strategy.hpp>

namespace ramen
{
namespace ui
{
namespace
{

class move2d_manipulator_t : public manipulator_t
{
public:

	move2d_manipulator_t( param_t *param) : node_( 0)
	{
		param_ = dynamic_cast<transform2_param_t*>( param);
		RAMEN_ASSERT( param_);

		drag_center_ = drag_axes_ = false;
		dragx_ = dragy_ = true;
	}

private:

	virtual void do_draw_overlay( const ui::paint_event_t& event) const
	{
		RAMEN_ASSERT( node()->composition());

		image::move2d_node_t::matrix3_type lm( param_->matrix_at_frame( node()->composition()->frame(), node()->aspect_ratio()));

		Imath::V2f p = get_absolute_value<Imath::V2f>( param_->center_param());
	    p = p * lm;
		p.x *= node()->aspect_ratio();

		gl_line_width( default_line_width());
	    manipulators::draw_xy_axes( p, 70 / event.pixel_scale, 70 / event.pixel_scale,
									get_value<float>( param_->rotate_param()),
									event.aspect_ratio / node()->aspect_ratio(),
									  palette_t::instance().color("x_axis"),
									  palette_t::instance().color("y_axis"), event.pixel_scale);

	    manipulators::draw_cross( p, 3 / event.pixel_scale, 3 / event.pixel_scale, default_color(), event.pixel_scale);
		manipulators::draw_ellipse( p, 7 / event.pixel_scale, 7 / event.pixel_scale, Imath::Color3c( 255, 255, 255), 20);

		// draw the boundary
		image::move2d_node_t::matrix3_type gm( node()->global_matrix());
		gm *= image::move2d_node_t::matrix3_type().setScale( image::move2d_node_t::vector2_type( node()->aspect_ratio(), 1));

	    Imath::Box2i bbox( node()->format());
		++bbox.max.x;
		++bbox.max.y;

		float offset = manipulators::shadow_offset( event.pixel_scale);
		gl_line_width( manipulator_t::default_line_width());
		gl_point_size( manipulator_t::default_control_point_size());

		// shadow
		gl_color3ub( 0, 0, 0);
	    gl_begin( GL_LINE_LOOP);
			manipulators::gl_transformed_box( bbox, gm, offset);
	    gl_end();

		// color
		gl_color( manipulator_t::default_color());
	    gl_begin( GL_LINE_LOOP);
			manipulators::gl_transformed_box( bbox, gm);
	    gl_end();
	}

	virtual bool do_key_press_event( const ui::key_press_event_t& event) { return false;}
	virtual void do_key_release_event( const ui::key_release_event_t& event) {}

	virtual bool do_mouse_press_event( const ui::mouse_press_event_t& event)
	{
		RAMEN_ASSERT( node()->composition());

		drag_center_ = drag_axes_ = false;
		dragx_ = dragy_ = true;

		image::move2d_node_t::matrix3_type m( param_->matrix_at_frame( node()->composition()->frame(), node()->aspect_ratio()));

	    // adjust for the aspect factor
	    Imath::V2f c = get_absolute_value<Imath::V2f>( param_->center_param());
	    c = c * m;
		c.x *= node()->aspect_ratio();

		switch( manipulators::pick_xy_axes( event.wpos, c, 70 / event.pixel_scale, 70 / event.pixel_scale,
											get_value<float>( param_->rotate_param()),
											event.aspect_ratio / node()->aspect_ratio(), event.pixel_scale))
		{
			case manipulators::axes_center_picked:
			{
				if( event.modifiers & ui::event_t::control_modifier)
					drag_center_ = true;
				else
					drag_axes_ = true;
			}
			return true;

			case manipulators::axes_x_picked:
			{
				dragy_ = false;

				if( event.modifiers & ui::event_t::control_modifier)
					drag_center_ = true;
				else
					drag_axes_ = true;
			}
			return true;

			case manipulators::axes_y_picked:
			{
				dragx_ = false;

				if( event.modifiers & ui::event_t::control_modifier)
					drag_center_ = true;
				else
					drag_axes_ = true;
			}
			return true;

			default:
				return false;
		}
	}

	virtual void do_mouse_drag_event( const ui::mouse_drag_event_t& event)
	{
		RAMEN_ASSERT( drag_axes_ || drag_center_);

		if( event.first_drag)
			param_->param_set()->begin_edit();

		Imath::V2f off( event.wpos - event.last_wpos);
		off.x /= event.aspect_ratio;

		if( drag_center_)
			param_->move_center( off);
		else
			param_->translate( off);

		if( param_->track_mouse())
			param_->param_set()->notify_parent();
		else
			event.view->update();
	}

	virtual void do_mouse_release_event( const ui::mouse_release_event_t& event)
	{
		if( event.dragged)
		{
			RAMEN_ASSERT( drag_axes_ || drag_center_);
			param_->param_set()->end_edit( param_->track_mouse());
		}

		drag_center_ = drag_axes_ = false;
		dragx_ = dragy_ = true;
	}

	const image::move2d_node_t *node() const
	{
		if( !node_)
			get_node();

		return node_;
	}

	image::move2d_node_t *node()
	{
		if( !node_)
			get_node();

		return node_;
	}

	void get_node() const
	{
		const image::move2d_node_t *n = dynamic_cast<const image::move2d_node_t*>( target());
		RAMEN_ASSERT( n);
		node_ = const_cast<image::move2d_node_t*>( n);
	}

	mutable image::move2d_node_t *node_;
	transform2_param_t *param_;
	bool drag_center_;
	bool drag_axes_;
	bool dragx_, dragy_;
};

std::unique_ptr<manipulable_t> create_twirl_overlay( node_t& n)
{
	std::unique_ptr<manipulable_t> overlay( new manipulable_t( n));
	float_param_t *radius = dynamic_cast<float_param_t*>( &n.param( "radius"));
	float2_param_t *center = dynamic_cast<float2_param_t*>( &n.param( "center"));
	overlay->add_manipulator( std::unique_ptr<circle_manipulator_t>( new circle_manipulator_t( radius, center)));
	return overlay;
}

std::unique_ptr<manipulable_t> create_gradient_overlay( node_t& n)
{
	std::unique_ptr<manipulable_t> overlay( new manipulable_t( n));
    overlay->add_manipulator( std::unique_ptr<point2_manipulator_t>( new point2_manipulator_t( dynamic_cast<float2_param_t*>( &n.param( "start")))));
    overlay->add_manipulator( std::unique_ptr<point2_manipulator_t>( new point2_manipulator_t( dynamic_cast<float2_param_t*>( &n.param( "end")))));
	return overlay;
}

std::unique_ptr<manipulable_t> create_rgradient_overlay( node_t& n)
{
	std::unique_ptr<manipulable_t> overlay( new manipulable_t( n));
    overlay->add_manipulator( std::unique_ptr<point2_manipulator_t>( new point2_manipulator_t( dynamic_cast<float2_param_t*>( &n.param( "center")))));
	return overlay;
}

std::unique_ptr<manipulable_t> create_move2d_overlay( node_t& n)
{
	std::unique_ptr<manipulable_t> overlay( new manipulable_t( n));
    overlay->add_manipulator( std::unique_ptr<move2d_manipulator_t>( new move2d_manipulator_t( &n.param( "xf"))));
	return overlay;
}

std::unique_ptr<manipulable_t> create_roto_overlay( node_t& n)
{
	image::roto_node_t *roto = dynamic_cast<image::roto_node_t*>( &n);
	RAMEN_ASSERT( roto);
	return std::unique_ptr<manipulable_t>( new roto::roto_overlay_t( *roto));
}

} // unnamed

void register_builtin_overlays()
{
	register_overlay( "image.builtin.twirl", &create_twirl_overlay);
	register_overlay( "image.builtin.gradient", &create_gradient_overlay);
	register_overlay( "image.builtin.rgradient", &create_rgradient_overlay);
	register_overlay( "image.builtin.move2d", &create_move2d_overlay);
	register_overlay( "image.builtin.roto", &create_roto_overlay);
}

} // ui
} // ramen
