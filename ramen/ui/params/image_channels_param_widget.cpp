// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/image_channels_param_widget.hpp>

#include<ramen/nodes/image/input/image_channels_param.hpp>
#include<algorithm>

#include<QPushButton>
#include<QVBoxLayout>
#include<QLabel>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/inspector/inspector.hpp>

#include<ramen/app/application.hpp>
#include<ramen/params/param_set.hpp>
#include<ramen/params/parameterised.hpp>

namespace ramen
{
namespace ui
{

image_channels_param_widget_t::image_channels_param_widget_t( image_channels_param_t& p) : param_widget_t( p), param_( p) {}

QWidget *image_channels_param_widget_t::do_create_widgets()
{
    QWidget *top = new QWidget();
    int current_height = 0;

    // popups
    {
		QLabel *label = new QLabel( top);
		red_combo_ = new QComboBox( top);
		red_combo_->setFocusPolicy( Qt::NoFocus);
		QSize s = red_combo_->sizeHint();

		label->move( 0, current_height);
		label->resize( app().ui()->inspector().left_margin() - 5, s.height());
		label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
		label->setText( "Red");

		red_combo_->move( app().ui()->inspector().left_margin(), current_height);
		red_combo_->resize( app().ui()->inspector().width() - app().ui()->inspector().left_margin() - 10, s.height());
		connect( red_combo_, SIGNAL( currentIndexChanged( int)), this, SLOT( channel_picked( int)));
		current_height = s.height() + 5;
    }

    {
		QLabel *label = new QLabel( top);
		green_combo_ = new QComboBox( top);
		green_combo_->setFocusPolicy( Qt::NoFocus);

		QSize s = green_combo_->sizeHint();

		label->move( 0, current_height);
		label->resize( app().ui()->inspector().left_margin() - 5, s.height());
		label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
		label->setText( "Green");

		green_combo_->move( app().ui()->inspector().left_margin(), current_height);
		green_combo_->resize( app().ui()->inspector().width() - app().ui()->inspector().left_margin() - 10, s.height());
		connect( green_combo_, SIGNAL( currentIndexChanged( int)), this, SLOT( channel_picked( int)));
		current_height += s.height() + 5;
    }

    {
		QLabel *label = new QLabel( top);
		blue_combo_ = new QComboBox( top);
		blue_combo_->setFocusPolicy( Qt::NoFocus);

		QSize s = blue_combo_->sizeHint();

		label->move( 0, current_height);
		label->resize( app().ui()->inspector().left_margin() - 5, s.height());
		label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
		label->setText( "Blue");

		blue_combo_->move( app().ui()->inspector().left_margin(), current_height);
		blue_combo_->resize( app().ui()->inspector().width() - app().ui()->inspector().left_margin() - 10, s.height());
		connect( blue_combo_, SIGNAL( currentIndexChanged( int)), this, SLOT( channel_picked( int)));
		current_height += s.height() + 5;
    }

    {
		QLabel *label = new QLabel( top);
		alpha_combo_ = new QComboBox( top);
		alpha_combo_->setFocusPolicy( Qt::NoFocus);

		QSize s = alpha_combo_->sizeHint();

		label->move( 0, current_height);
		label->resize( app().ui()->inspector().left_margin() - 5, s.height());
		label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
		label->setText( "Alpha");

		alpha_combo_->move( app().ui()->inspector().left_margin(), current_height);
		alpha_combo_->resize( app().ui()->inspector().width() - app().ui()->inspector().left_margin() - 10, s.height());
		connect( alpha_combo_, SIGNAL( currentIndexChanged( int)), this, SLOT( channel_picked( int)));
		current_height += s.height() + 5;
    }

    top->setMinimumSize( app().ui()->inspector().width(), current_height);
    top->setMaximumSize( app().ui()->inspector().width(), current_height);
    top->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);

	tuple4i_t ch( get_value<tuple4i_t>( param_));
	update_popup( red_combo_   , std::get<0>( ch));
	update_popup( green_combo_ , std::get<1>( ch));
	update_popup( blue_combo_  , std::get<2>( ch));
	update_popup( alpha_combo_ , std::get<3>( ch));
    return top;
}

void image_channels_param_widget_t::do_update_widgets()
{
	// the channel list may have changed, rebuild the menus.
	if( red_combo_)
	{
		tuple4i_t ch( get_value<tuple4i_t>( param_));
		update_popup( red_combo_   , std::get<0>( ch));
		update_popup( green_combo_ , std::get<1>( ch));
		update_popup( blue_combo_  , std::get<2>( ch));
		update_popup( alpha_combo_ , std::get<3>( ch));
	}
}

void image_channels_param_widget_t::do_enable_widgets( bool e)
{
	if( red_combo_)
	{
		red_combo_->setEnabled( e);
		green_combo_->setEnabled( e);
		blue_combo_->setEnabled( e);
		alpha_combo_->setEnabled( e);
	}
}

void image_channels_param_widget_t::update_popup( QComboBox *popup, int value)
{
    popup->blockSignals( true);
	popup->clear();
	
    if( !param_.channel_list_.empty())
    {
		for( int i = 0; i < param_.channel_list_.size(); ++i)
			popup->addItem( param_.channel_list_[i].c_str());

		popup->addItem( "None");
		popup->setEnabled( true);
    }
    else
    {
		popup->addItem( "Red");
		popup->addItem( "Green");
		popup->addItem( "Blue");
		popup->addItem( "Alpha");
		popup->setEnabled( false);
    }

    popup->setCurrentIndex( value);
    popup->blockSignals( false);
}

void image_channels_param_widget_t::channel_picked( int index)
{
	tuple4i_t ch( get_value<tuple4i_t>( param_));

    if( QObject::sender() == red_combo_)
		std::get<0>( ch) = index;

    if( QObject::sender() == green_combo_)
		std::get<1>( ch) = index;

    if( QObject::sender() == blue_combo_)
		std::get<2>( ch) = index;

    if( QObject::sender() == alpha_combo_)
		std::get<3>( ch) = index;

    param_.param_set()->begin_edit();
    param_.param_set()->add_command( &param_);
    param_.value().assign( ch);
	param_.emit_param_changed( param_t::user_edited);
    param_.param_set()->end_edit();
}

} // ui
} // ramen
