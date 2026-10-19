// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/motion_blur_param_widget.hpp>

#include<ramen/params/motion_blur_param.hpp>

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

motion_blur_param_widget_t::motion_blur_param_widget_t( motion_blur_param_t& p) : param_widget_t( p), param_( p) {}

QWidget *motion_blur_param_widget_t::do_create_widgets()
{
    motion_blur_info_t info( get_value<motion_blur_info_t>( param_));

    QWidget *top = new QWidget();

    shutter_ = new ui::double_spinbox_t( top);
    shutter_->setRange( 0, 1);
    shutter_->setSingleStep( 0.05);

    QSize s = shutter_->sizeHint();
    s.setWidth( s.width() + ( s.width() / 3));
    int h = 0;

    QLabel *label = new QLabel( top);
    label->move( 0, h);
    label->resize( app().ui()->inspector().left_margin() - 5, s.height());
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
    label->setText( "Motion Blur Samples");

    samples_ = new ui::double_spinbox_t( top);
    samples_->setDecimals( 0);
    samples_->setTrackMouse( false);
    samples_->setRange( 1, 64);
    samples_->setValue( info.samples);
    samples_->move( app().ui()->inspector().left_margin(), h);
    samples_->resize( s.width(), s.height());
    samples_->setEnabled( param_.enabled());
    connect( samples_, SIGNAL( valueChanged( double)), this, SLOT( samples_changed( double)));
    h += s.height() + 5;

    label = new QLabel( top);
    label->move( 0, h);
    label->resize( app().ui()->inspector().left_margin() - 5, s.height());
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
    label->setText( "Shutter");

    shutter_->setTrackMouse( false);
    shutter_->setValue( info.shutter);
    shutter_->move( app().ui()->inspector().left_margin(), h);
    shutter_->resize( s.width(), s.height());
    shutter_->setEnabled( param_.enabled());
    connect( shutter_, SIGNAL( valueChanged( double)), this, SLOT( shutter_changed( double)));
    h += s.height() + 5;

    label = new QLabel( top);
    label->move( 0, h);
    label->resize( app().ui()->inspector().left_margin() - 5, s.height());
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
    label->setText( "Shutter Offset");

    shutter_off_ = new ui::double_spinbox_t( top);
    shutter_off_->setTrackMouse( false);
    shutter_off_->setRange( -1, 1);
    shutter_off_->setValue( info.shutter_offset);
    shutter_off_->setSingleStep( 0.05);
    shutter_off_->move( app().ui()->inspector().left_margin(), h);
    shutter_off_->resize( s.width(), s.height());
    shutter_off_->setEnabled( param_.enabled());
    connect( shutter_off_, SIGNAL( valueChanged( double)), this, SLOT( shutter_changed( double)));
    h += s.height() + 5;

    label = new QLabel( top);
    label->move( 0, h);
    label->resize( app().ui()->inspector().left_margin() - 5, s.height());
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
    label->setText( "Filter");

    filter_ = new QComboBox( top);
    s = filter_->sizeHint();
    filter_->setFocusPolicy( Qt::NoFocus);
    filter_->addItem( "Box");
    filter_->addItem( "Triangle");
    filter_->addItem( "Cubic");
    filter_->setCurrentIndex( (int) info.filter);
    filter_->move( app().ui()->inspector().left_margin(), h);
    filter_->resize( s.width(), s.height());
    filter_->setEnabled( param_.enabled());
    connect( filter_, SIGNAL( currentIndexChanged( int)), this, SLOT( filter_changed( int)));
    h += s.height() + 5;

    top->setMinimumSize( app().ui()->inspector().width(), h);
    top->setMaximumSize( app().ui()->inspector().width(), h);
    top->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    return top;
}

void motion_blur_param_widget_t::do_update_widgets()
{
    if( samples_)
    {
        motion_blur_info_t info( get_value<motion_blur_info_t>( param_));

        samples_->blockSignals( true);
        shutter_->blockSignals( true);
        shutter_off_->blockSignals( true);
        filter_->blockSignals( true);

        samples_->setValue( info.samples);
        shutter_->setValue( info.shutter);
        shutter_off_->setValue( info.shutter_offset);
        filter_->setCurrentIndex( (int) info.filter);

        samples_->blockSignals( false);
        shutter_->blockSignals( false);
        shutter_off_->blockSignals( false);
        filter_->blockSignals( false);
    }
}

void motion_blur_param_widget_t::do_enable_widgets( bool e)
{
    if( samples_)
    {
        samples_->setEnabled( e);
        shutter_->setEnabled( e);
        shutter_off_->setEnabled( e);
        filter_->setEnabled( e);
    }
}

void motion_blur_param_widget_t::samples_changed( double v)
{
    motion_blur_info_t info( get_value<motion_blur_info_t>( param_));
    info.samples = v;
    param_.param_set()->begin_edit();
    param_.param_set()->add_command( &param_);
    param_.value().assign( info);
    param_.emit_param_changed( param_t::user_edited);
    param_.param_set()->end_edit();
}

void motion_blur_param_widget_t::shutter_changed( double v)
{
    motion_blur_info_t info( get_value<motion_blur_info_t>( param_));
    info.shutter = shutter_->value();
    info.shutter_offset = shutter_off_->value();
    param_.param_set()->begin_edit();
    param_.param_set()->add_command( &param_);
    param_.value().assign( info);
    param_.emit_param_changed( param_t::user_edited);
    param_.param_set()->end_edit();
}

void motion_blur_param_widget_t::filter_changed( int index)
{
    motion_blur_info_t info( get_value<motion_blur_info_t>( param_));
    info.filter = (motion_blur_info_t::filter_type) index;
    param_.param_set()->begin_edit();
    param_.param_set()->add_command( &param_);
    param_.value().assign( info);
    param_.emit_param_changed( param_t::user_edited);
    param_.param_set()->end_edit();
}

} // ui
} // ramen
