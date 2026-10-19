// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/param_widget.hpp>

#include<vector>

#include<QWidget>

namespace ramen
{
namespace ui
{
namespace
{

std::vector<param_widget_creator_t>& creators()
{
    static std::vector<param_widget_creator_t> c;
    return c;
}

} // unnamed

param_widget_t::param_widget_t( param_t& p) : QObject()
{
    update_connection_ = p.update_widgets_signal.connect( [this]() { do_update_widgets();});
    enable_connection_ = p.enable_widgets_signal.connect( [this]( bool e) { do_enable_widgets( e);});
}

param_widget_t::~param_widget_t()
{
    update_connection_.disconnect();
    enable_connection_.disconnect();
}

QWidget *param_widget_t::create_widgets() { return do_create_widgets();}

void param_widget_t::do_update_widgets() {}
void param_widget_t::do_enable_widgets( bool e) {}

void register_param_widget( const param_widget_creator_t& creator)
{
    creators().push_back( creator);
}

QWidget *create_param_widgets( param_t& p)
{
    if( p.secret())
        return 0;

    // later registrations are for more specific param classes.
    for( auto it( creators().rbegin()); it != creators().rend(); ++it)
    {
        if( param_widget_t *w = ( *it)( p))
        {
            QWidget *top = w->create_widgets();

            if( !top)
            {
                delete w;
                return 0;
            }

            w->setParent( top);
            return top;
        }
    }

    return 0;
}

} // ui
} // ramen
