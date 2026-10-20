// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/nodes/image/transform/move2d_node.hpp>
#include<ramen/app/palette.hpp>

#include<functional>

#include<ramen/assert.hpp>

#include<ramen/params/numeric_param.hpp>
#include<ramen/params/transform2_param.hpp>
#include<ramen/params/separator_param.hpp>
#include<ramen/params/button_param.hpp>

#include<ramen/app/composition.hpp>




namespace ramen
{
namespace image
{

move2d_node_t::move2d_node_t() : xform2d_node_t()
{ 
	set_name("move2d");
    param_set().param_changed.connect( std::bind( &move2d_node_t::param_changed, this, std::placeholders::_1, std::placeholders::_2));
}

move2d_node_t::move2d_node_t( const move2d_node_t& other) : xform2d_node_t( other)
{
    param_set().param_changed.connect( std::bind( &move2d_node_t::param_changed, this, std::placeholders::_1, std::placeholders::_2));
}

void move2d_node_t::do_create_params()
{
    std::unique_ptr<transform2_param_t> xp( new transform2_param_t( "xform", "xf"));
    add_param( std::move( xp));

	create_default_filter_param();

    std::unique_ptr<button_param_t> b( new button_param_t( "Apply Track..."));
    b->set_id( "apply_track");
    add_param( std::move( b));

    std::unique_ptr<separator_param_t> sep( new separator_param_t());
    add_param( std::move( sep));

	create_motion_blur_param();
}

move2d_node_t::matrix3_type move2d_node_t::do_calc_transform_matrix_at_frame( float frame, int subsample) const
{
    const transform2_param_t *p = dynamic_cast<const transform2_param_t*>( &param( "xf"));
    RAMEN_ASSERT( p);
    return p->matrix_at_frame( frame, aspect_ratio(), subsample);
}

void move2d_node_t::param_changed( param_t *p, param_t::change_reason reason)
{
	RAMEN_ASSERT( composition());
	
	if( reason == param_t::user_edited && p == &param( "apply_track"))
	{
        /*
		apply_track_mode mode;
		apply_track_use use;
		
		const tracker_node_t *tracker = ui::apply_track_dialog_t::instance().exec( mode,  use);
		
		if( tracker)
		{
			transform2_param_t *xf = dynamic_cast<transform2_param_t*>( &param( "xf"));
			xf->apply_track( composition()->start_frame(), composition()->end_frame(), tracker, mode, use);
			update_widgets();
		}
        */
	}
}

node_t *create_move2d_node() { return new move2d_node_t();}

const node_metaclass_t *move2d_node_t::metaclass() const { return &move2d_node_metaclass();}

const node_metaclass_t& move2d_node_t::move2d_node_metaclass()
{
    static bool inited( false);
    static node_metaclass_t info;

    if( !inited)
    {
        info.id = "image.builtin.move2d";
        info.major_version = 1;
        info.minor_version = 0;
        info.menu = "Image";
        info.submenu = "Transform";
        info.menu_item = "Move2D";
        info.create = &create_move2d_node;
        inited = true;
    }

    return info;
}

static bool registered = node_factory_t::instance().register_node( move2d_node_t::move2d_node_metaclass());

} // namespace
} // namespace
