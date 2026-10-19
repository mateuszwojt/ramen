// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_MOTION_BLUR_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_MOTION_BLUR_PARAM_WIDGET_HPP

#include<ramen/ui/params/param_widget.hpp>

#include<QPointer>
#include<ramen/ui/widgets/double_spinbox_fwd.hpp>

#include<ramen/params/motion_blur_param.hpp>

class QComboBox;

namespace ramen
{
namespace ui
{

class motion_blur_param_widget_t : public param_widget_t
{
    Q_OBJECT

public:

    explicit motion_blur_param_widget_t( motion_blur_param_t& p);

private:

    void do_update_widgets() override;
    void do_enable_widgets( bool e) override;
    QWidget *do_create_widgets() override;

    motion_blur_param_t& param_;

    QPointer<ui::double_spinbox_t> shutter_, shutter_off_, samples_;
    QPointer<QComboBox> filter_;

private Q_SLOTS:

    void samples_changed( double v);
    void shutter_changed( double v);
    void filter_changed( int index);
};

} // ui
} // ramen

#endif
