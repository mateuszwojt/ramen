// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/aspect_ratio_param_widget.hpp>

#include<ramen/params/aspect_ratio_param.hpp>

#include<QComboBox>
#include<QLabel>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/inspector/inspector.hpp>
#include<ramen/ui/widgets/double_spinbox.hpp>

#include<ramen/app/application.hpp>
#include<ramen/params/param_set.hpp>
#include<ramen/params/parameterised.hpp>

namespace ramen
{
namespace ui
{

aspect_ratio_param_widget_t::aspect_ratio_param_widget_t( aspect_ratio_param_t& p) : param_widget_t( p), param_( p) {}

void aspect_ratio_param_widget_t::do_update_widgets()
{
    if( menu_)
    {
        menu_->blockSignals( true);
        input_->blockSignals( true);

        float val = get_value<float>( param_);
        input_->setValue( val);
        menu_->setCurrentIndex( param_.index_for_value( val));

        menu_->blockSignals( false);
        input_->blockSignals( false);
    }
}

void aspect_ratio_param_widget_t::do_enable_widgets( bool e)
{
    if( menu_)
    {
        menu_->setEnabled( e);
        input_->setEnabled( e);
    }
}

QWidget *aspect_ratio_param_widget_t::do_create_widgets()
{
    QWidget *top = new QWidget();
    QLabel *label = new QLabel( top);
    menu_ = new QComboBox( top);
    input_ = new ui::double_spinbox_t( top);

    QSize s = input_->sizeHint();

    label->move( 0, 0);
    label->resize( app().ui()->inspector().left_margin() - 5, s.height());
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
    label->setText( param_.name().c_str());
    label->setToolTip( param_.id().c_str());

    input_->move( app().ui()->inspector().left_margin(), 0);
    float val = get_value<float>( param_);
    input_->setValue( val);
    input_->setEnabled( param_.enabled());
    input_->setDecimals( 3);
    input_->setMinimum( 0.1);
    input_->setTrackMouse( false);
    connect( input_, SIGNAL( valueChanged( double)), this, SLOT( value_changed( double)));

    menu_->setFocusPolicy( Qt::NoFocus);

    for( int i = 0; i < param_.presets().size(); ++i)
        menu_->addItem( param_.presets()[i].first.c_str());

    menu_->addItem( "Custom");

    menu_->move( app().ui()->inspector().left_margin() + s.width() + 5, 0);
    menu_->setEnabled( param_.enabled());
    menu_->setCurrentIndex( param_.index_for_value( val));
    connect( menu_, SIGNAL( currentIndexChanged( int)), this, SLOT( item_picked( int)));

    top->setMinimumSize( app().ui()->inspector().width(), s.height());
    top->setMaximumSize( app().ui()->inspector().width(), s.height());
    top->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    return top;
}

void aspect_ratio_param_widget_t::item_picked( int index)
{
    if( index == param_.presets().size())
        return;

    param_.param_set()->begin_edit();
    float val = param_.presets()[index].second;
    param_.set_value( val);

    input_->blockSignals( true);
    input_->setValue( val);
    input_->blockSignals( false);

    param_.param_set()->end_edit();
}

void aspect_ratio_param_widget_t::value_changed( double value)
{
    param_.param_set()->begin_edit();
    param_.set_value( value);

    menu_->blockSignals( true);
    menu_->setCurrentIndex( param_.index_for_value( value));
    menu_->blockSignals( false);

    param_.param_set()->end_edit();
}

} // ui
} // ramen
