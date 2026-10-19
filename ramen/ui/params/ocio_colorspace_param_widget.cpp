// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/ocio_colorspace_param_widget.hpp>

#include<ramen/params/ocio_colorspace_param.hpp>
#include<ramen/nodes/node.hpp>
#include<ramen/ocio/manager.hpp>

#include<QComboBox>
#include<QLabel>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/inspector/inspector.hpp>
#include<ramen/ui/widgets/ocio_colorspace_combo.hpp>

#include<ramen/app/application.hpp>
#include<ramen/params/param_set.hpp>
#include<ramen/params/parameterised.hpp>

namespace ramen
{
namespace ui
{

ocio_colorspace_param_widget_t::ocio_colorspace_param_widget_t( ocio_colorspace_param_t& p) : param_widget_t( p), param_( p) {}

QWidget *ocio_colorspace_param_widget_t::do_create_widgets()
{
    QWidget *top = new QWidget();
    QLabel *label = new QLabel( top);
    menu_ = new ui::ocio_colorspace_combo_t( top);
    menu_->setFocusPolicy( Qt::NoFocus);

    QSize s = menu_->sizeHint();
    label->move( 0, 0);
    label->resize( app().ui()->inspector().left_margin() - 5, s.height());
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
    label->setText( param_.name().c_str());
    label->setToolTip( param_.id().c_str());

    menu_->move( app().ui()->inspector().left_margin(), 0);
    menu_->resize( s.width(), s.height());

    std::string current_colorspace = get_value<std::string>( param_);
    menu_->set_colorspace_or_default( current_colorspace);
    menu_->setEnabled( param_.enabled());
    connect( menu_, SIGNAL( colorspace_changed( const std::string&)), this, SLOT( colorspace_picked( const std::string&)));

    top->setMinimumSize( app().ui()->inspector().width(), s.height());
    top->setMaximumSize( app().ui()->inspector().width(), s.height());
    top->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    return top;
}

void ocio_colorspace_param_widget_t::do_update_widgets()
{
    if( menu_)
    {
        menu_->blockSignals( true);

        std::string csname = get_value<std::string>( param_);
        menu_->set_colorspace( csname);
        menu_->blockSignals( false);
    }
}

void ocio_colorspace_param_widget_t::do_enable_widgets( bool e)
{
    if( menu_)
        menu_->setEnabled( e);
}

void ocio_colorspace_param_widget_t::colorspace_picked( const std::string& cs)
{
    param_.param_set()->begin_edit();
    param_.set_value( menu_->get_current_colorspace());
    param_.param_set()->end_edit();
}

} // ui
} // ramen
