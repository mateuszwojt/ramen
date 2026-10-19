// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_COLOR_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_COLOR_PARAM_WIDGET_HPP

#include<ramen/ui/params/param_widget.hpp>

#include<QPointer>
#include<QToolButton>
#include<ramen/ui/widgets/color.hpp>
#include<ramen/ui/widgets/param_spinbox_fwd.hpp>
#include<ramen/ui/widgets/color_button_fwd.hpp>
#include<ramen/ui/widgets/eyedropper_button_fwd.hpp>

#include<ramen/params/color_param.hpp>

namespace ramen
{
namespace ui
{

class color_param_widget_t : public param_widget_t
{
    Q_OBJECT

public:

    explicit color_param_widget_t( color_param_t& p);

private:

    void do_update_widgets() override;
    void do_enable_widgets( bool e) override;
    QWidget *do_create_widgets() override;

    color_param_t& param_;

   QPointer<ui::param_spinbox_t> input0_, input1_, input2_, input3_;
   QPointer<ui::color_button_t> button_;
   QPointer<ui::eyedropper_button_t> eyedropper_;

    void set_component_value_from_slot();

private Q_SLOTS:

    void value_changed( double value);
    void spinbox_pressed();
    void spinbox_dragged( double value);
    void spinbox_released();
    void eyedropper_color_picked( const ramen::ui::color_t& c);
    void color_button_pressed();
};

} // ui
} // ramen

#endif
