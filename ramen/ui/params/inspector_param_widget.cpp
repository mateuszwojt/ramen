// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/inspector_param_widget.hpp>

#include<ramen/app/application.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/inspector/inspector.hpp>
#include<ramen/ui/inspector/panel.hpp>

namespace ramen
{
namespace ui
{

inspector_param_widget_t::inspector_param_widget_t( inspector_param_t& p) : param_widget_t( p), param_( p)
{
    connection_ = p.contents_changed.connect( [this]() { contents_changed();});
}

inspector_param_widget_t::~inspector_param_widget_t() { connection_.disconnect();}

QWidget *inspector_param_widget_t::do_create_widgets()
{
	widget_ = new ui::container_widget_t();
    widget_->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Preferred);
    widget_->setMinimumSize( app().ui()->inspector().width()+16, 0);
    widget_->setMaximumSize( app().ui()->inspector().width()+16, QWIDGETSIZE_MAX);
    widget_->setContentsMargins( 0, 0, 0, 0);

    set_contents();
	return widget_;
}

void inspector_param_widget_t::contents_changed()
{
    if( !widget_)
        return;

    widget_->clear_contents();
    set_contents();
}

void inspector_param_widget_t::set_contents()
{
	if( parameterised_t *p = param_.parameterised())
	{
		QWidget *w = app().ui()->inspector().panel_factory().create_panel( p)->second->widget();
		widget_->set_contents( w);
	}
}

} // ui
} // ramen
