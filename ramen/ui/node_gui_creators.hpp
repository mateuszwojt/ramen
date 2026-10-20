// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_NODE_GUI_CREATORS_HPP
#define RAMEN_UI_NODE_GUI_CREATORS_HPP

#include<ramen/config.hpp>

namespace ramen
{
namespace ui
{

/// Registers the functions that create nodes interactively, like asking for a file.
RAMEN_API void register_node_gui_creators();

} // ui
} // ramen

#endif
