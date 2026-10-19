// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/composite_param_widget.hpp>

#include<ramen/params/composite_param.hpp>
#include<string>
#include<stdexcept>
#include<functional>
#include<algorithm>
#include<ramen/anim/track.hpp>

#include<QWidget>
#include<QVBoxLayout>
#include<QFrame>

#include<ramen/ui/inspector/inspector.hpp>

#include<ramen/app/application.hpp>
#include<ramen/params/param_set.hpp>
#include<ramen/params/parameterised.hpp>
#include<ramen/ui/user_interface.hpp>

namespace ramen
{
namespace ui
{

composite_param_widget_t::composite_param_widget_t( composite_param_t& p) : param_widget_t( p), param_( p) {}

QWidget *composite_param_widget_t::do_create_widgets()
{
    QWidget *widget = new QWidget();
    create_param_widgets_inside_widget( param_, widget);
    return widget;
}

void create_param_widgets_inside_widget( composite_param_t& param, QWidget *parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins( 0, 0, 0, 0);
    layout->setSpacing( 5);
    layout->setSizeConstraint( QLayout::SetFixedSize);

    for( param_t& p : param.params())
    {
        if( !p.secret())
        {
            QWidget *w = create_param_widgets( p);

            if( w)
                layout->addWidget( w);
        }
    }

    layout->addStretch();
    parent->setLayout( layout);
}

} // ui
} // ramen
