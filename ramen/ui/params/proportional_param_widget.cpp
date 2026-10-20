// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/proportional_param_widget.hpp>

#include<QIcon>

namespace ramen
{
namespace ui
{

proportional_param_widget_t::proportional_param_widget_t( proportional_param_t& p) : param_widget_t( p), prop_param_( p) {}

void proportional_param_widget_t::create_proportional_button( QWidget *parent, int height)
{
    prop_button_ = new QToolButton( parent);
    prop_button_->setCheckable( true);
    prop_button_->setChecked( prop_param_.proportional_checked());
    prop_button_->setIcon( QIcon( prop_param_.proportional_checked() ? ":icons/link.png" : ":icons/unlink.png"));
    prop_button_->setEnabled( prop_param_.enabled());
	prop_button_->resize( height, height);
	connect( prop_button_, SIGNAL( toggled( bool)), this, SLOT( proportional_toggle(bool)));
}

void proportional_param_widget_t::proportional_toggle( bool state)
{
    prop_param_.set_proportional_checked( state);

	if( state)
		prop_button_->setIcon( QIcon( ":icons/link.png"));
	else
		prop_button_->setIcon( QIcon( ":icons/unlink.png"));
}

} // ui
} // ramen
