// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/app/ui_notifications.hpp>

namespace ramen
{

ui_notifications_t& ui_notifications()
{
    static ui_notifications_t n;
    return n;
}

} // ramen
