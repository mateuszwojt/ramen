// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_IMAGE_FORMAT_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_IMAGE_FORMAT_PARAM_WIDGET_HPP

#include<ramen/ui/params/param_widget.hpp>

#include<QPointer>
#include<ramen/ui/widgets/double_spinbox_fwd.hpp>

#include<ramen/params/image_format_param.hpp>

class QComboBox;

namespace ramen
{
namespace ui
{

class image_format_param_widget_t : public param_widget_t
{
    Q_OBJECT

public:

    explicit image_format_param_widget_t( image_format_param_t& p);

private:

    void do_update_widgets() override;
    void do_enable_widgets( bool e) override;
    QWidget *do_create_widgets() override;

    image_format_param_t& param_;

    QPointer<ui::double_spinbox_t> width_input_, height_input_, aspect_input_;
    QPointer<QComboBox> menu_;

private Q_SLOTS:

    void preset_picked( int v);
    void set_new_format( double unused = 0);
};

} // ui
} // ramen

#endif
