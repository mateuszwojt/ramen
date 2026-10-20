// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/tab_group_param_widget.hpp>

#include<ramen/params/tab_group_param.hpp>
#include<functional>
#include<algorithm>

#include<QTabWidget>

#include<ramen/ui/inspector/inspector.hpp>

#include<ramen/app/application.hpp>
#include<ramen/params/param_set.hpp>
#include<ramen/params/parameterised.hpp>
#include<ramen/ui/user_interface.hpp>

namespace ramen
{
namespace ui
{

tab_group_param_widget_t::tab_group_param_widget_t( tab_group_param_t& p) : param_widget_t( p), param_( p) {}

QWidget *tab_group_param_widget_t::do_create_widgets()
{
    tab_ = new QTabWidget();

    for( param_t& p : param_.params())
    {
		QWidget *w = create_param_widgets( p);

		if( w)
		    tab_->addTab( w, p.name().c_str());
    }

    tab_->setCurrentIndex( 0);
    return tab_;
}

void tab_group_param_widget_t::do_enable_widgets( bool e)
{
    if( tab_)
		tab_->setEnabled( e);
}

} // ui
} // ramen
