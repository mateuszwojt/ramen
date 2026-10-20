// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_MANIPULATORS_OVERLAYS_HPP
#define RAMEN_UI_MANIPULATORS_OVERLAYS_HPP

#include<ramen/config.hpp>

#include<functional>
#include<memory>
#include<string>

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/ui/manipulators/manipulable.hpp>

namespace ramen
{
namespace ui
{

typedef std::function<std::unique_ptr<manipulable_t>( node_t&)> overlay_creator_t;

/// Registers the function that creates the viewer overlay of the nodes of a class.
RAMEN_API void register_overlay( const std::string& node_class_id, const overlay_creator_t& creator);

/// Creates the viewer overlay of a node. Returns null if its class has none.
RAMEN_API std::unique_ptr<manipulable_t> create_overlay( node_t& n);

/// Registers the overlays of the builtin nodes.
RAMEN_API void register_builtin_overlays();

} // ui
} // ramen

#endif
