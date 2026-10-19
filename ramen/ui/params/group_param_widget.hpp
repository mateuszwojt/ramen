// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_GROUP_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_GROUP_PARAM_WIDGET_HPP

#include<ramen/ui/params/param_widget.hpp>

#include<QPointer>

#include<ramen/params/group_param.hpp>

class QGroupBox;

namespace ramen
{
namespace ui
{

class group_param_widget_t : public param_widget_t
{
    Q_OBJECT

public:

    explicit group_param_widget_t( group_param_t& p);

private:

    QWidget *do_create_widgets() override;
    void do_enable_widgets( bool e) override;

    group_param_t& param_;

    QPointer<QGroupBox> group_;

};

} // ui
} // ramen

#endif
