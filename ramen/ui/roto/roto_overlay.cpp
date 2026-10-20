// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/roto/roto_overlay.hpp>

#include<ramen/nodes/image/roto/roto_node.hpp>

#include<ramen/ui/roto/manipulator.hpp>
#include<ramen/ui/roto/toolbar.hpp>

namespace ramen
{
namespace roto
{

roto_overlay_t::roto_overlay_t( image::roto_node_t& node) : manipulable_t( node), node_( node)
{
    add_manipulator( std::unique_ptr<roto::manipulator_t>( new roto::manipulator_t()));
}

std::unique_ptr<QWidget> roto_overlay_t::create_toolbar()
{
	toolbar_ = new roto::toolbar_t( node_);
	return std::unique_ptr<QWidget>( toolbar_.data());
}

toolbar_t *roto_overlay_t::toolbar() const { return toolbar_.data();}

} // roto
} // ramen
