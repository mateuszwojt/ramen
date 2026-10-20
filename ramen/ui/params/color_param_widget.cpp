// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/color_param_widget.hpp>

#include<ramen/params/color_param.hpp>
#include<limits>
#include<cmath>
#include<functional>
#include<ramen/assert.hpp>
#include<ramen/algorithm/clamp.hpp>
#include<ramen/nodes/node.hpp>
#include<ramen/app/document.hpp>
#include<ramen/anim/track.hpp>

#include<QLabel>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/main_window.hpp>
#include<ramen/ui/inspector/inspector.hpp>
#include<ramen/ui/anim/anim_editor.hpp>
#include<ramen/ui/widgets/param_spinbox.hpp>
#include<ramen/ui/widgets/color_button.hpp>
#include<ramen/ui/widgets/eyedropper_button.hpp>
#include<ramen/ui/widgets/color_picker.hpp>

#include<ramen/app/application.hpp>
#include<ramen/params/param_set.hpp>
#include<ramen/params/parameterised.hpp>

namespace ramen
{
namespace ui
{

color_param_widget_t::color_param_widget_t( color_param_t& p) : param_widget_t( p), param_( p) {}

void color_param_widget_t::do_update_widgets()
{
    if( input0_)
    {
        input0_->blockSignals( true);
        input1_->blockSignals( true);
        input2_->blockSignals( true);

        if( input3_)
            input3_->blockSignals( true);

        button_->blockSignals( true);

        Imath::Color4f col = get_value<Imath::Color4f>( param_);
        input0_->setValue( col.r);
        input1_->setValue( col.g);
        input2_->setValue( col.b);

        if( input3_)
            input3_->setValue( col.a);

        ui::color_t c( col.r, col.g, col.b, col.a);
        c.apply_gamma( 1.0 / 2.2);
        button_->set_value( c);

        input0_->blockSignals( false);
        input1_->blockSignals( false);
        input2_->blockSignals( false);

        if( input3_)
            input3_->blockSignals( false);

        button_->blockSignals( false);
    }
}

void color_param_widget_t::do_enable_widgets( bool e)
{
    if( input0_)
    {
        input0_->setEnabled( e);
        input1_->setEnabled( e);
        input2_->setEnabled( e);

        if( input3_)
            input3_->setEnabled( e);

        button_->setEnabled( e);
        eyedropper_->setEnabled( e);
    }
}

QWidget *color_param_widget_t::do_create_widgets()
{
    QWidget *top = new QWidget();
    QLabel *label = new QLabel( top);
    button_ = new ui::color_button_t( top);

    input0_ = new ui::param_spinbox_t( param_, 0, top);
    input1_ = new ui::param_spinbox_t( param_, 1, top);
    input2_ = new ui::param_spinbox_t( param_, 2, top);

    if( param_.is_rgba())
        input3_ = new ui::param_spinbox_t(param_, 3, top);

    QSize s = input0_->sizeHint();

    label->move( 0, 0);
    label->resize( app().ui()->inspector().left_margin() - 5, s.height());
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
    label->setText( param_.name().c_str());
    label->setToolTip( param_.id().c_str());
    Imath::Color4f col = get_value<Imath::Color4f>( param_);

    int xpos = app().ui()->inspector().left_margin();

    button_->move( xpos, 0);
    button_->resize( s.height(), s.height());
    button_->set_value( ui::color_t( std::pow( (double) col.r, 1.0 / 2.2),
                                      std::pow( (double) col.g, 1.0 / 2.2),
                                        std::pow( (double) col.b, 1.0 / 2.2)));

    button_->setEnabled( param_.enabled());
    connect( button_, SIGNAL( pressed()), this, SLOT( color_button_pressed()));
    xpos += s.height();

    eyedropper_ = new ui::eyedropper_button_t( top);
    eyedropper_->move( xpos, 0);
    eyedropper_->resize( s.height(), s.height());
    eyedropper_->setEnabled( param_.enabled());
    connect( eyedropper_, SIGNAL( color_picked( const ramen::ui::color_t&)), this, SLOT( eyedropper_color_picked( const ramen::ui::color_t&)));
    xpos += s.height() + 3;

    // make spinboxes a bit smaller
    s.setWidth( s.width() - 10);

    input0_->move( xpos, 0);
    input0_->resize( s.width(), s.height());
    input0_->setMinimum( 0);
    input0_->setDecimals( 4);
    input0_->setValue( col.r);
    input0_->setSingleStep( param_.step());
    input0_->setEnabled( param_.enabled());
    connect( input0_, SIGNAL( valueChanged(double)), button_, SLOT( set_red(double)));
    connect( input0_, SIGNAL( valueChanged( double)), this, SLOT( value_changed( double)));
    connect( input0_, SIGNAL( spinBoxPressed()), this, SLOT( spinbox_pressed()));
    connect( input0_, SIGNAL( spinBoxDragged( double)), this, SLOT( spinbox_dragged( double)));
    connect( input0_, SIGNAL( spinBoxReleased()), this, SLOT( spinbox_released()));
    xpos += s.width() + 3;

    input1_->move( xpos, 0);
    input1_->resize( s.width(), s.height());
    input1_->setMinimum( 0);
    input1_->setDecimals( 4);
    input1_->setValue( col.g);
    input1_->setSingleStep( param_.step());
    input1_->setEnabled( param_.enabled());
    connect( input1_, SIGNAL( valueChanged(double)), button_, SLOT( set_green(double)));
    connect( input1_, SIGNAL( valueChanged( double)), this, SLOT( value_changed( double)));
    connect( input1_, SIGNAL( spinBoxPressed()), this, SLOT( spinbox_pressed()));
    connect( input1_, SIGNAL( spinBoxDragged( double)), this, SLOT( spinbox_dragged( double)));
    connect( input1_, SIGNAL( spinBoxReleased()), this, SLOT( spinbox_released()));
    xpos += s.width() + 3;

    input2_->move( xpos, 0);
    input2_->resize( s.width(), s.height());
    input2_->setMinimum( 0);
    input2_->setDecimals( 4);
    input2_->setValue( col.b);
    input2_->setSingleStep( param_.step());
    input2_->setEnabled( param_.enabled());
    connect( input2_, SIGNAL( valueChanged(double)), button_, SLOT( set_blue(double)));
    connect( input2_, SIGNAL( valueChanged( double)), this, SLOT( value_changed( double)));
    connect( input2_, SIGNAL( spinBoxPressed()), this, SLOT( spinbox_pressed()));
    connect( input2_, SIGNAL( spinBoxDragged( double)), this, SLOT( spinbox_dragged( double)));
    connect( input2_, SIGNAL( spinBoxReleased()), this, SLOT( spinbox_released()));
    xpos += s.width() + 3;

    if( param_.is_rgba())
    {
        input3_->move( xpos, 0);
        input3_->resize( s.width(), s.height());
        input3_->setValue( col.a);
        input3_->setRange( 0, 1);
        input3_->setSingleStep( param_.step());
        input3_->setEnabled( param_.enabled());
        connect( input3_, SIGNAL( valueChanged( double)), this, SLOT( value_changed( double)));
        connect( input3_, SIGNAL( spinBoxPressed()), this, SLOT( spinbox_pressed()));
        connect( input3_, SIGNAL( spinBoxDragged( double)), this, SLOT( spinbox_dragged( double)));
        connect( input3_, SIGNAL( spinBoxReleased()), this, SLOT( spinbox_released()));
        xpos += s.width() + 3;
    }

    top->setMinimumSize( app().ui()->inspector().width(), s.height());
    top->setMaximumSize( app().ui()->inspector().width(), s.height());
    top->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    return top;
}

void color_param_widget_t::value_changed( double value)
{
    param_.param_set()->begin_edit();
    set_component_value_from_slot();
    param_.param_set()->end_edit();
}

void color_param_widget_t::spinbox_pressed()
{
    if( param_.track_mouse())
        app().ui()->begin_interaction();

    param_.param_set()->begin_edit();
}

void color_param_widget_t::spinbox_dragged( double value)
{
    set_component_value_from_slot();

    if( param_.track_mouse())
        param_.param_set()->notify_parent();

    if( app().ui())
        app().ui()->update_anim_editors();
}

void color_param_widget_t::spinbox_released()
{
    param_.param_set()->end_edit( !param_.track_mouse());

    if( param_.track_mouse())
        app().ui()->end_interaction();
}

void color_param_widget_t::eyedropper_color_picked( const ramen::ui::color_t& c)
{
    param_.param_set()->begin_edit();

    if( param_.is_rgba())
        param_.set_value( Imath::Color4f( c.red(), c.green(), c.blue(), c.alpha()));
    else
        param_.set_value( Imath::Color4f( c.red(), c.green(), c.blue(), 1));

    param_.update_widgets();
    param_.param_set()->end_edit();
}

void color_param_widget_t::color_button_pressed()
{
    Imath::Color4f col = get_value<Imath::Color4f>( param_);
    ui::color_t c( col.r, col.g, col.b);
    ui::color_picker_t *picker = new ui::color_picker_t( app().ui()->main_window(), c);

    if( picker->exec() == QDialog::Accepted)
    {
        c = picker->color();

        param_.param_set()->begin_edit();

        if( param_.is_rgba())
            param_.set_value( Imath::Color4f( c.red(), c.green(), c.blue(), col.a));
        else
            param_.set_value( Imath::Color4f( c.red(), c.green(), c.blue(), 1));

        param_.update_widgets();
        param_.param_set()->end_edit();
    }

    delete picker;
}

void color_param_widget_t::set_component_value_from_slot()
{
    Imath::Color4f v( input0_->value(), input1_->value(), input2_->value(), 1.0f);

    if( param_.is_rgba())
        v.a = input3_->value();

    int index;
    float comp_value;

    if( QObject::sender() == input0_)
    {
        index = 0;
        comp_value = v.r;
    }
    else
    {
        if( QObject::sender() == input1_)
        {
            index = 1;
            comp_value = v.g;
        }
        else
        {
            if( QObject::sender() == input2_)
            {
                index = 2;
                comp_value = v.b;
            }
            else
            {
                RAMEN_ASSERT( param_.is_rgba());

                index = 3;
                comp_value = v.a;
            }
        }
    }

    param_.set_component_value( index, comp_value);
}

} // ui
} // ramen
