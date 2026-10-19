// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_INSPECTOR_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_INSPECTOR_PARAM_WIDGET_HPP

#include<ramen/ui/params/param_widget.hpp>

#include<QPointer>

#include<ramen/params/inspector_param.hpp>

#include<ramen/ui/widgets/container_widget.hpp>

namespace ramen
{
namespace ui
{

/// Shows the panel of the parameterised contained in an inspector param.
class inspector_param_widget_t : public param_widget_t
{
    Q_OBJECT

public:

    explicit inspector_param_widget_t( inspector_param_t& p);
    ~inspector_param_widget_t();

private:

    QWidget *do_create_widgets() override;
    void contents_changed();
    void set_contents();

    inspector_param_t& param_;

    QPointer<ui::container_widget_t> widget_;
    core::connection_t connection_;
};

} // ui
} // ramen

#endif
