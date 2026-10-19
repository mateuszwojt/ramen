// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/group_param_widget.hpp>

#include<ramen/params/group_param.hpp>

#include<ramen/ui/params/composite_param_widget.hpp>

#include<QGroupBox>
#include<QVBoxLayout>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/inspector/inspector.hpp>
#include<ramen/ui/widgets/group_box.hpp>

#include<ramen/app/application.hpp>
#include<ramen/params/param_set.hpp>
#include<ramen/params/parameterised.hpp>

namespace ramen
{
namespace ui
{

group_param_widget_t::group_param_widget_t( group_param_t& p) : param_widget_t( p), param_( p) {}

QWidget *group_param_widget_t::do_create_widgets()
{
    if( param_.can_collapse_)
        group_ = new ui::group_box_t();
    else
        group_ = new QGroupBox();

    group_->setTitle( param_.name().c_str());
    group_->setAlignment( Qt::AlignLeft);
    create_param_widgets_inside_widget( param_, group_);

    group_->setMinimumSize( app().ui()->inspector().width(), 20);
    return group_;
}

void group_param_widget_t::do_enable_widgets( bool e)
{
    if( group_)
        group_->setEnabled( e);
}

} // ui
} // ramen
