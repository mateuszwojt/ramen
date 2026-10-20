// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/combo_group_param_widget.hpp>

#include<ramen/params/combo_group_param.hpp>
#include<algorithm>
#include<ramen/nodes/node.hpp>
#include<ramen/params/static_param_command.hpp>
#include<ramen/serialization/yaml_oarchive.hpp>

#include<QStackedWidget>
#include<QComboBox>
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

combo_group_param_widget_t::combo_group_param_widget_t( combo_group_param_t& p) : param_widget_t( p), param_( p) {}

void combo_group_param_widget_t::do_update_widgets()
{
    if( menu_)
    {
        menu_->blockSignals( true);
        menu_->setCurrentIndex( get_value<int>( param_));
        stack_->setCurrentIndex( get_value<int>( param_));
        menu_->blockSignals( false);
    }
}

void combo_group_param_widget_t::do_enable_widgets( bool e)
{
    if( menu_)
    {
        menu_->setEnabled( e);
        stack_->setEnabled( e);
    }
}

QWidget *combo_group_param_widget_t::do_create_widgets()
{
    QWidget *top = new QWidget();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins( 0, 0, 0, 0);
    layout->setSpacing( 5);
    layout->setSizeConstraint( QLayout::SetFixedSize);

    QWidget *selector = new QWidget();
    QLabel *label = new QLabel( selector);

    menu_ = new QComboBox( selector);
    menu_->setFocusPolicy( Qt::NoFocus);

    stack_ = new QStackedWidget();

    for( param_t& p : param_.params())
    {
        QWidget *w = create_param_widgets( p);

        if( w)
        {
            stack_->addWidget( w);
            menu_->addItem( p.name().c_str());
        }
    }

    QSize s = menu_->sizeHint();

    label->move( 0, 0);
    label->resize( app().ui()->inspector().left_margin() - 5, s.height());
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
    label->setText( param_.name().c_str());
    label->setToolTip( param_.id().c_str());

    menu_->move( app().ui()->inspector().left_margin(), 0);
    menu_->resize( s.width(), s.height());
    menu_->setEnabled( param_.enabled());

    menu_->setCurrentIndex( get_value<int>( param_));
    stack_->setCurrentIndex( get_value<int>( param_));
    stack_->setEnabled( param_.enabled());
    connect( menu_, SIGNAL( currentIndexChanged( int)), this, SLOT( item_picked( int)));

    selector->setMinimumSize( app().ui()->inspector().width(), s.height());
    selector->setMaximumSize( app().ui()->inspector().width(), s.height());
    selector->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);

    layout->addWidget( selector);
    layout->addWidget( stack_);

    top->setLayout( layout);
    return top;
}

void combo_group_param_widget_t::item_picked( int index)
{
    param_.param_set()->begin_edit();
    param_.set_value( index);

    if( stack_)
        stack_->setCurrentIndex( index);

    param_.param_set()->end_edit();
}

} // ui
} // ramen
