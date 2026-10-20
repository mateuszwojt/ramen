// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_APP_UI_NOTIFICATIONS_HPP
#define RAMEN_APP_UI_NOTIFICATIONS_HPP

#include<ramen/config.hpp>

#include<ramen/core/signal.hpp>

#include<ramen/nodes/node_fwd.hpp>

namespace ramen
{

/*!
\ingroup app
\brief Signals the core emits when the user interface needs to refresh.
The user interface connects to them, without one they do nothing.
*/
struct RAMEN_API ui_notifications_t
{
    /// The composition or the active node changed, redraw the views.
    core::signal_t<void ()> update;

    /// Animation curves changed.
    core::signal_t<void ()> update_anim_editors;

    /// The animation tracks of a node changed.
    core::signal_t<void ( node_t*)> recreate_anim_tracks;
};

RAMEN_API ui_notifications_t& ui_notifications();

} // ramen

#endif
