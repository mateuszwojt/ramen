// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/bool_param_widget.hpp>

#include<ramen/params/bool_param.hpp>
#include<ramen/nodes/node.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/inspector/inspector.hpp>

#include<ramen/app/application.hpp>
#include<ramen/params/param_set.hpp>
#include<ramen/params/parameterised.hpp>

namespace ramen
{
namespace ui
{

bool_param_widget_t::bool_param_widget_t( bool_param_t& p) : param_widget_t( p), param_( p) {}

void bool_param_widget_t::do_update_widgets()
{
    if( button_)
    {
        button_->blockSignals( true);
        button_->setChecked( get_value<bool>( param_));
        button_->blockSignals( false);
    }
}

void bool_param_widget_t::do_enable_widgets( bool e)
{
    if( button_)
        button_->setEnabled( e);
}

QWidget *bool_param_widget_t::do_create_widgets()
{
    QWidget *w = new QWidget();

    button_ = new QCheckBox( w);
    button_->setFocusPolicy( Qt::NoFocus);
    button_->setText( param_.name().c_str());
    QSize s = button_->sizeHint();

    w->setMinimumSize( app().ui()->inspector().width(), s.height());
    w->setMaximumSize( app().ui()->inspector().width(), s.height());

    button_->move( app().ui()->inspector().left_margin(), 0);
    button_->resize( s.width(), s.height());
    button_->setChecked( get_value<bool>( param_));
    button_->setEnabled( param_.enabled());
    button_->setToolTip( param_.id().c_str());

    connect( button_, SIGNAL( stateChanged( int)), this, SLOT( button_checked( int)));
    return w;
}

void bool_param_widget_t::button_checked( int state)
{
    param_.param_set()->begin_edit();
    param_.set_value( state);
    param_.param_set()->end_edit();
}

} // ui
} // ramen
