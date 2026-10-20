// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_IMAGE_CHANNELS_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_IMAGE_CHANNELS_PARAM_WIDGET_HPP

#include<ramen/ui/params/param_widget.hpp>

#include<QPointer>
#include<QComboBox>

#include<ramen/nodes/image/input/image_channels_param.hpp>

namespace ramen
{
namespace ui
{

class image_channels_param_widget_t : public param_widget_t
{
    Q_OBJECT

public:

    explicit image_channels_param_widget_t( image_channels_param_t& p);

private:

    void do_update_widgets() override;
    void do_enable_widgets( bool e) override;
    void update_popup( QComboBox *popup, int value);
    QWidget *do_create_widgets() override;

    image_channels_param_t& param_;

    QPointer<QComboBox> red_combo_, green_combo_, blue_combo_, alpha_combo_;

private Q_SLOTS:

    void channel_picked( int index);
};

} // ui
} // ramen

#endif
