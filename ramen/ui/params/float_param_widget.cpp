// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/float_param_widget.hpp>

#include<ramen/params/float_param.hpp>
#include<limits>
#include<functional>
#include<ramen/app/composition.hpp>
#include<ramen/serialization/yaml_node.hpp>
#include<ramen/serialization/yaml_oarchive.hpp>

#include<QLabel>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/anim/anim_editor.hpp>
#include<ramen/ui/viewer/viewer.hpp>
#include<ramen/ui/inspector/inspector.hpp>
#include<ramen/ui/widgets/param_spinbox.hpp>

#include<ramen/app/application.hpp>
#include<ramen/params/param_set.hpp>
#include<ramen/params/parameterised.hpp>

namespace ramen
{
namespace ui
{

float_param_widget_t::float_param_widget_t( float_param_t& p) : param_widget_t( p), param_( p) {}

void float_param_widget_t::do_update_widgets()
{
    if( input_)
    {
        input_->blockSignals( true);
        input_->setValue( param_.relative_to_absolute( get_value<float>( param_)));
        input_->blockSignals( false);
    }
}

void float_param_widget_t::do_enable_widgets( bool e)
{
    if( input_)
        input_->setEnabled( e);
}

QWidget *float_param_widget_t::do_create_widgets()
{
    QWidget *top = new QWidget();
    QLabel *label = new QLabel( top);
    input_ = new ui::param_spinbox_t( param_, 0, top);

    QSize s = input_->sizeHint();

    label->move( 0, 0);
    label->resize( app().ui()->inspector().left_margin() - 5, s.height());
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
    label->setText( param_.name().c_str());
    label->setToolTip( param_.id().c_str());

    float low = param_.absolute_min();
    float high = param_.absolute_max();

    input_->setRange( low, high);

    input_->setValue( param_.relative_to_absolute( get_value<float>( param_)));

    input_->setSingleStep( param_.step());
    input_->move( app().ui()->inspector().left_margin(), 0);
    input_->resize( s.width(), s.height());
    input_->setEnabled( param_.enabled());

    if( param_.round_to_int())
        input_->setDecimals( 0);

    connect( input_, SIGNAL( valueChanged( double)), this, SLOT( value_changed( double)));
    connect( input_, SIGNAL( spinBoxPressed()), this, SLOT( spinbox_pressed()));
    connect( input_, SIGNAL( spinBoxDragged( double)), this, SLOT( spinbox_dragged( double)));
    connect( input_, SIGNAL( spinBoxReleased()), this, SLOT( spinbox_released()));

    top->setMinimumSize( app().ui()->inspector().width(), s.height());
    top->setMaximumSize( app().ui()->inspector().width(), s.height());
    top->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    return top;
}

void float_param_widget_t::value_changed( double value)
{
    param_.param_set()->begin_edit();
    param_.set_value( param_.absolute_to_relative( param_.round( value)));
    param_.param_set()->end_edit();
}

void float_param_widget_t::spinbox_pressed()
{
    if( param_.track_mouse())
        app().ui()->begin_interaction();

    param_.param_set()->begin_edit();
}

void float_param_widget_t::spinbox_dragged( double value)
{
    param_.set_value( param_.absolute_to_relative( param_.round( value)));

    if( param_.track_mouse())
        param_.param_set()->notify_parent();
    else
        param_.parameterised()->update_overlay();

    if( app().ui())
        app().ui()->update_anim_editors();
}

void float_param_widget_t::spinbox_released()
{
    param_.param_set()->end_edit( !param_.track_mouse());

    if( param_.track_mouse())
        app().ui()->end_interaction();
}

} // ui
} // ramen
