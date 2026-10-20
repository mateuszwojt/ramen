// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_COMPOSITE_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_COMPOSITE_PARAM_WIDGET_HPP

#include<ramen/ui/params/param_widget.hpp>

#include<QPointer>

#include<ramen/params/composite_param.hpp>

namespace ramen
{
namespace ui
{

class composite_param_widget_t : public param_widget_t
{
    Q_OBJECT

public:

    explicit composite_param_widget_t( composite_param_t& p);

private:

    QWidget *do_create_widgets() override;

    composite_param_t& param_;

};

/// Creates the widgets for the children of p in a vertical layout inside parent.
void create_param_widgets_inside_widget( composite_param_t& p, QWidget *parent);

} // ui
} // ramen

#endif
