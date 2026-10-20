// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_SEPARATOR_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_SEPARATOR_PARAM_WIDGET_HPP

#include<ramen/ui/params/param_widget.hpp>

#include<QPointer>

#include<ramen/params/separator_param.hpp>

namespace ramen
{
namespace ui
{

class separator_param_widget_t : public param_widget_t
{
    Q_OBJECT

public:

    explicit separator_param_widget_t( separator_param_t& p);

private:

    QWidget *do_create_widgets() override;

    separator_param_t& param_;

};

} // ui
} // ramen

#endif
