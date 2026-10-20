// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/separator_param_widget.hpp>

#include<ramen/params/separator_param.hpp>

#include<QFrame>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/inspector/inspector.hpp>

#include<ramen/app/application.hpp>
#include<ramen/params/param_set.hpp>
#include<ramen/params/parameterised.hpp>

namespace ramen
{
namespace ui
{

separator_param_widget_t::separator_param_widget_t( separator_param_t& p) : param_widget_t( p), param_( p) {}

QWidget *separator_param_widget_t::do_create_widgets()
{
    QFrame *separator = new QFrame();
    separator->setFrameStyle( QFrame::HLine | QFrame::Raised);
    separator->setLineWidth( 1);
    QSize s = separator->sizeHint();

    separator->setMinimumSize( app().ui()->inspector().width(), s.height());
    separator->setMaximumSize( app().ui()->inspector().width(), s.height());
    separator->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    return separator;
}

} // ui
} // ramen
