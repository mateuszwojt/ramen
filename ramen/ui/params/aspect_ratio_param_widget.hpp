// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_ASPECT_RATIO_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_ASPECT_RATIO_PARAM_WIDGET_HPP

#include<ramen/ui/params/param_widget.hpp>

#include<QPointer>
#include<ramen/ui/widgets/double_spinbox_fwd.hpp>

#include<ramen/params/aspect_ratio_param.hpp>

class QComboBox;

namespace ramen
{
namespace ui
{

class aspect_ratio_param_widget_t : public param_widget_t
{
    Q_OBJECT

public:

    explicit aspect_ratio_param_widget_t( aspect_ratio_param_t& p);

private:

    void do_update_widgets() override;
    void do_enable_widgets( bool e) override;
    QWidget *do_create_widgets() override;

    aspect_ratio_param_t& param_;

    QPointer<QComboBox> menu_;
    QPointer<ui::double_spinbox_t> input_;

private Q_SLOTS:

    void item_picked( int index);
    void value_changed( double value);
};

} // ui
} // ramen

#endif
