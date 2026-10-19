// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/float3_param_widget.hpp>

#include<ramen/params/float3_param.hpp>
#include<limits>
#include<functional>
#include<ramen/algorithm/clamp.hpp>
#include<ramen/nodes/node.hpp>
#include<ramen/app/document.hpp>

#include<QLabel>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/anim/anim_editor.hpp>
#include<ramen/ui/inspector/inspector.hpp>
#include<ramen/ui/viewer/viewer.hpp>
#include<ramen/ui/widgets/param_spinbox.hpp>

#include<ramen/app/application.hpp>
#include<ramen/params/param_set.hpp>
#include<ramen/params/parameterised.hpp>

namespace ramen
{
namespace ui
{

float3_param_widget_t::float3_param_widget_t( float3_param_t& p) : proportional_param_widget_t( p), param_( p) {}

QWidget *float3_param_widget_t::do_create_widgets()
{
    QWidget *top = new QWidget();
    QLabel *label = new QLabel( top);
    input0_ = new ui::param_spinbox_t( param_, 0, top);
    input1_ = new ui::param_spinbox_t( param_, 1, top);
    input2_ = new ui::param_spinbox_t( param_, 2, top);

    QSize s = input0_->sizeHint();

    label->move( 0, 0);
    label->resize( app().ui()->inspector().left_margin() - 5, s.height());
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
    label->setText( param_.name().c_str());
    label->setToolTip( param_.id().c_str());
    Imath::V3f triple = get_value<Imath::V3f>( param_);

    // make inputs bigger
    s.setWidth( s.width() + ( s.width() / 4));

    int xpos = app().ui()->inspector().left_margin();

    input0_->move( xpos, 0);
    input0_->resize( s.width(), s.height());
    input0_->setRange( param_.get_min(), param_.get_max());
    input0_->setSingleStep( param_.step());
    input0_->setValue( triple.x);
    input0_->setEnabled( param_.enabled());
    if( param_.round_to_int()) input0_->setDecimals( 0);
    connect( input0_, SIGNAL( valueChanged( double)), this, SLOT( value_changed( double)));
    connect( input0_, SIGNAL( spinBoxPressed()), this, SLOT( spinbox_pressed()));
    connect( input0_, SIGNAL( spinBoxDragged( double)), this, SLOT( spinbox_dragged( double)));
    connect( input0_, SIGNAL( spinBoxReleased()), this, SLOT( spinbox_released()));
    xpos += ( s.width() + 5);

    input1_->move( xpos, 0);
    input1_->resize( s.width(), s.height());
    input1_->setRange( param_.get_min(), param_.get_max());
    input1_->setSingleStep( param_.step());
    input1_->setValue( triple.y);
    input1_->setEnabled( param_.enabled());
    if( param_.round_to_int())	input1_->setDecimals( 0);
    connect( input1_, SIGNAL( valueChanged( double)), this, SLOT( value_changed( double)));
    connect( input1_, SIGNAL( spinBoxPressed()), this, SLOT( spinbox_pressed()));
    connect( input1_, SIGNAL( spinBoxDragged( double)), this, SLOT( spinbox_dragged( double)));
    connect( input1_, SIGNAL( spinBoxReleased()), this, SLOT( spinbox_released()));
    xpos += ( s.width() + 5);

    input2_->move( xpos, 0);
    input2_->resize( s.width(), s.height());
    input2_->setRange( param_.get_min(), param_.get_max());
    input2_->setSingleStep( param_.step());
    input2_->setValue( triple.z);
    input2_->setEnabled( param_.enabled());
    if( param_.round_to_int()) input2_->setDecimals( 0);
    connect( input2_, SIGNAL( valueChanged( double)), this, SLOT( value_changed( double)));
    connect( input2_, SIGNAL( spinBoxPressed()), this, SLOT( spinbox_pressed()));
    connect( input2_, SIGNAL( spinBoxDragged( double)), this, SLOT( spinbox_dragged( double)));
    connect( input2_, SIGNAL( spinBoxReleased()), this, SLOT( spinbox_released()));
    xpos += ( s.width() + 2);

    if( param_.proportional())
    {
        create_proportional_button( top, s.height());

        if( prop_button_)
            prop_button_->move( xpos, 0);
    }

    top->setMinimumSize( app().ui()->inspector().width(), s.height());
    top->setMaximumSize( app().ui()->inspector().width(), s.height());
    top->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    return top;
}

void float3_param_widget_t::do_update_widgets()
{
    if( input0_)
    {
        input0_->blockSignals( true);
        input1_->blockSignals( true);
        input2_->blockSignals( true);

        Imath::V3f triple = get_value<Imath::V3f>( param_);
        input0_->setValue( triple.x);
        input1_->setValue( triple.y);
        input2_->setValue( triple.z);

        input0_->blockSignals( false);
        input1_->blockSignals( false);
        input2_->blockSignals( false);
    }
}

void float3_param_widget_t::do_enable_widgets( bool e)
{
    if( input0_)
    {
        input0_->setEnabled( e);
        input1_->setEnabled( e);
        input2_->setEnabled( e);

        if( prop_button_)
            prop_button_->setEnabled( e);
    }
}

void float3_param_widget_t::value_changed( double value)
{
    param_.param_set()->begin_edit();

    if( param_.proportional_checked())
    {
        calc_proportional_factors();
        Imath::V3f v = get_value<Imath::V3f>( param_);

        if( sender() == input0_)
        {
            float inc = value - v.x;
            v.x = value;
            v.y = param_.clamp( v.y + ( inc * param_.proportional_factor.y));
            v.z = param_.clamp( v.z + ( inc * param_.proportional_factor.z));
        }
        else
        {
            if( sender() == input1_)
            {
                float inc = value - v.y;
                v.x = param_.clamp( v.x + ( inc * param_.proportional_factor.x));
                v.y = value;
                v.z = param_.clamp( v.z + ( inc * param_.proportional_factor.z));
            }
            else
            {
                float inc = value - v.z;
                v.x = param_.clamp( v.x + ( inc * param_.proportional_factor.x));
                v.y = param_.clamp( v.y + ( inc * param_.proportional_factor.y));
                v.z = value;
            }
        }

        param_.set_value( param_.round( v));
        param_.update_widgets();
    }
    else
        set_component_value_from_slot();

    param_.param_set()->end_edit();
}

void float3_param_widget_t::spinbox_pressed()
{
    if( param_.track_mouse())
        app().ui()->begin_interaction();

    param_.param_set()->begin_edit();

    if( param_.proportional_checked())
        calc_proportional_factors();
}

void float3_param_widget_t::spinbox_dragged( double value)
{
    if( param_.proportional_checked())
    {
        Imath::V3f v = get_value<Imath::V3f>( param_);

        if( sender() == input0_)
        {
            float inc = value - v.x;
            v.x = value;
            v.y = ramen::clamp( v.y + ( inc * param_.proportional_factor.y), param_.get_min(), param_.get_max());
            v.z = ramen::clamp( v.z + ( inc * param_.proportional_factor.z), param_.get_min(), param_.get_max());
        }
        else
        {
            if( sender() == input1_)
            {
                float inc = value - v.y;
                v.x = ramen::clamp( v.x + ( inc * param_.proportional_factor.x), param_.get_min(), param_.get_max());
                v.y = value;
                v.z = ramen::clamp( v.z + ( inc * param_.proportional_factor.z), param_.get_min(), param_.get_max());
            }
            else
            {
                float inc = value - v.z;
                v.x = ramen::clamp( v.x + ( inc * param_.proportional_factor.x), param_.get_min(), param_.get_max());
                v.y = ramen::clamp( v.y + ( inc * param_.proportional_factor.y), param_.get_min(), param_.get_max());
                v.z = value;
            }
        }

        param_.set_value( param_.round ( v));
        param_.update_widgets();
    }
    else
        set_component_value_from_slot();

    if( param_.track_mouse())
        param_.param_set()->notify_parent();
    else
        param_.parameterised()->update_overlay();

    if( app().ui())
        app().ui()->update_anim_editors();
}

void float3_param_widget_t::spinbox_released()
{
    param_.param_set()->end_edit( !param_.track_mouse());

    if( param_.track_mouse())
        app().ui()->end_interaction();
}

void float3_param_widget_t::set_component_value_from_slot()
{
    Imath::V3f v( param_.round (input0_->value()), param_.round( input1_->value()), param_.round( input2_->value()));
    int index;
    float comp_value;

    if( QObject::sender() == input0_)
    {
        index = 0;
        comp_value = v.x;
    }
    else
    {
        if( QObject::sender() == input1_)
        {
            index = 1;
            comp_value = v.y;
        }
        else
        {
            index = 2;
            comp_value = v.z;
        }
    }

    param_.set_component_value( index, comp_value);
}

void float3_param_widget_t::calc_proportional_factors()
{
    param_.proportional_factor = Imath::V3f( 1, 1, 1);
    Imath::V3f v = get_value<Imath::V3f>( param_);

    if( sender() == input0_)
    {
        if( v.x)
        {
            param_.proportional_factor.y = ( v.y / v.x) != 0 ? v.y / v.x : 1;
            param_.proportional_factor.z = ( v.z / v.x) != 0 ? v.z / v.x : 1;
        }
    }
    else
    {
        if( sender() == input1_)
        {
            if( v.y)
            {
                param_.proportional_factor.x = ( v.x / v.y) != 0 ? v.x / v.y : 1;
                param_.proportional_factor.z = ( v.z / v.y) != 0 ? v.z / v.y : 1;
            }
        }
        else
        {
            if( v.z)
            {
                param_.proportional_factor.x = ( v.x / v.z) != 0 ? v.x / v.z : 1;
                param_.proportional_factor.y = ( v.y / v.z) != 0 ? v.y / v.z : 1;
            }
        }
    }
}

} // ui
} // ramen
