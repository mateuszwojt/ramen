// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_FLOAT_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_FLOAT_PARAM_WIDGET_HPP

#include<ramen/ui/params/param_widget.hpp>

#include<QPointer>
#include<ramen/ui/widgets/param_spinbox_fwd.hpp>

#include<ramen/params/float_param.hpp>

namespace ramen
{
namespace ui
{

class float_param_widget_t : public param_widget_t
{
    Q_OBJECT

public:

    explicit float_param_widget_t( float_param_t& p);

private:

    void do_update_widgets() override;
    void do_enable_widgets( bool e) override;
    QWidget *do_create_widgets() override;

    float_param_t& param_;

    QPointer<ui::param_spinbox_t> input_;

private Q_SLOTS:

    void value_changed( double value);
    void spinbox_pressed();
    void spinbox_dragged( double value);
    void spinbox_released();
};

} // ui
} // ramen

#endif
