// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/popup_param_widget.hpp>

#include<ramen/params/popup_param.hpp>
#include<ramen/serialization/yaml_oarchive.hpp>

#include<QComboBox>
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

popup_param_widget_t::popup_param_widget_t( popup_param_t& p) : param_widget_t( p), param_( p) {}

QWidget *popup_param_widget_t::do_create_widgets()
{
    QWidget *top = new QWidget();
    QLabel *label = new QLabel( top);
    menu_ = new QComboBox( top);
    menu_->setFocusPolicy( Qt::NoFocus);

    for( int i = 0; i < param_.menu_items_.size(); ++i)
    {
        if( param_.menu_items_[i] == "--")
            menu_->insertSeparator( menu_->count());
        else
            menu_->addItem( param_.menu_items_[i].c_str());
    }

    QSize s = menu_->sizeHint();

    label->move( 0, 0);
    label->resize( app().ui()->inspector().left_margin() - 5, s.height());
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
    label->setText( param_.name().c_str());
    label->setToolTip( param_.id().c_str());

    menu_->move( app().ui()->inspector().left_margin(), 0);
    menu_->resize( s.width(), s.height());
    menu_->setCurrentIndex( get_value<int>( param_));
    menu_->setEnabled( param_.enabled());
    connect( menu_, SIGNAL( currentIndexChanged( int)), this, SLOT( item_picked( int)));

    top->setMinimumSize( app().ui()->inspector().width(), s.height());
    top->setMaximumSize( app().ui()->inspector().width(), s.height());
    top->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    return top;
}

void popup_param_widget_t::do_update_widgets()
{
    if( menu_)
    {
        menu_->blockSignals( true);
        menu_->setCurrentIndex( get_value<int>( param_));
        menu_->blockSignals( false);
    }
}

void popup_param_widget_t::do_enable_widgets( bool e)
{
    if( menu_)
        menu_->setEnabled( e);
}

void popup_param_widget_t::item_picked( int index)
{
    param_.param_set()->begin_edit();
    param_.set_value( index);
    param_.param_set()->end_edit();
}

} // ui
} // ramen
