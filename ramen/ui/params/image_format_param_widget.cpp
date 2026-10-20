// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/image_format_param_widget.hpp>

#include<ramen/params/image_format_param.hpp>
#include<ramen/app/composition.hpp>

#include<QLabel>
#include<QComboBox>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/inspector/inspector.hpp>
#include<ramen/ui/widgets/spinbox.hpp>
#include<ramen/ui/widgets/double_spinbox.hpp>

#include<ramen/app/application.hpp>
#include<ramen/params/param_set.hpp>
#include<ramen/params/parameterised.hpp>

namespace ramen
{
namespace ui
{

image_format_param_widget_t::image_format_param_widget_t( image_format_param_t& p) : param_widget_t( p), param_( p) {}

QWidget *image_format_param_widget_t::do_create_widgets()
{
    QWidget *top = new QWidget();

    menu_ = new QComboBox( top);
    QSize s = menu_->sizeHint();

    QLabel *label = new QLabel( top);
    label->move( 0, 0);
    label->resize( app().ui()->inspector().left_margin() - 5, s.height());
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
    label->setText( param_.name().c_str());

    menu_->setFocusPolicy( Qt::NoFocus);

    for( int i = 0; i < image::format_t::presets().size(); ++i)
        menu_->addItem( image::format_t::presets()[i].first.c_str());

    menu_->addItem( "Custom");

    image::format_t format( get_value<image::format_t>( param_));

    menu_->move( app().ui()->inspector().left_margin(), 0);
    menu_->setCurrentIndex( format.preset_index());
    menu_->setEnabled( param_.enabled());
    connect( menu_, SIGNAL( currentIndexChanged( int)), this, SLOT( preset_picked(int)));
    int h = s.height() + 5;

    width_input_ = new ui::double_spinbox_t( top);
    width_input_->setRange( 16, app().preferences().max_image_width());
    width_input_->setDecimals( 0);
    width_input_->setTrackMouse( false);
    width_input_->setValue( param_.value().cast<image::format_t>().width);
    width_input_->move( app().ui()->inspector().left_margin(), h);
    width_input_->setMinimum( 16);
    width_input_->setValue( format.width);
    width_input_->setEnabled( param_.enabled());
    connect( width_input_, SIGNAL( valueChanged( double)), this, SLOT( set_new_format( double)));
    s = width_input_->sizeHint();

    height_input_ = new ui::double_spinbox_t( top);
    height_input_->setRange( 16, app().preferences().max_image_height());
    height_input_->setDecimals( 0);
    height_input_->setValue( param_.value().cast<image::format_t>().height);
    height_input_->setTrackMouse( false);
    height_input_->setEnabled( param_.enabled());
    height_input_->move( app().ui()->inspector().left_margin() + s.width() + 5, h);
    height_input_->setMinimum( 16);
    height_input_->setValue( format.height);
    connect( height_input_, SIGNAL( valueChanged( double)), this, SLOT( set_new_format(double)));

    aspect_input_ = new ui::double_spinbox_t( top);
    aspect_input_->setValue( 1);
    aspect_input_->setTrackMouse( false);
    aspect_input_->setEnabled( param_.enabled());
    aspect_input_->setValue( format.aspect);
    aspect_input_->setDecimals( 3);
    aspect_input_->setMinimum( 0.1);
    aspect_input_->setSingleStep( 0.05);
    aspect_input_->move( app().ui()->inspector().left_margin() + ( 2 * s.width()) + 10, h);
    connect( aspect_input_, SIGNAL( valueChanged( double)), this, SLOT( set_new_format(double)));
    h += s.height() + 5;

    int w = ( width_input_->sizeHint().width() * 2) + aspect_input_->sizeHint().width() + 10;
    menu_->resize( w, menu_->sizeHint().height());

    top->setMinimumSize( app().ui()->inspector().width(), h);
    top->setMaximumSize( app().ui()->inspector().width(), h);
    top->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    return top;
}

void image_format_param_widget_t::do_update_widgets()
{
    if( width_input_)
    {
        width_input_->blockSignals( true);
        height_input_->blockSignals( true);
        menu_->blockSignals( true);
        aspect_input_->blockSignals( true);

        image::format_t f( get_value<image::format_t>( param_));

        width_input_->setValue( f.width);
        height_input_->setValue( f.height);
        menu_->setCurrentIndex( f.preset_index());
        aspect_input_->setValue( f.aspect);

        width_input_->blockSignals( false);
        height_input_->blockSignals( false);
        menu_->blockSignals( false);
        aspect_input_->blockSignals( false);
    }
}

void image_format_param_widget_t::do_enable_widgets( bool e)
{
    if( width_input_)
    {
        width_input_->setEnabled( e);
        height_input_->setEnabled( e);
        menu_->setEnabled( e);
        aspect_input_->setEnabled( e);
    }
}

void image_format_param_widget_t::preset_picked( int v)
{
    if( v == image::format_t::presets().size())
        return;

    image::format_t new_format;
    new_format.set_from_preset( v);

    param_.param_set()->begin_edit();
    param_.set_value( new_format);
    param_.param_set()->end_edit();
    param_.update_widgets();
}

void image_format_param_widget_t::set_new_format( double unused)
{
    int w = width_input_->value();
    int h = height_input_->value();
    float a = aspect_input_->value();

    image::format_t new_format( w, h, a);

    menu_->blockSignals( true);
    menu_->setCurrentIndex( new_format.preset_index());
    menu_->blockSignals( false);

    param_.param_set()->begin_edit();
    param_.set_value( new_format);
    param_.param_set()->end_edit();
}

} // ui
} // ramen
