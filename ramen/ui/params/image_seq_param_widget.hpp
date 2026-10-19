// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_IMAGE_SEQ_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_IMAGE_SEQ_PARAM_WIDGET_HPP

#include<ramen/ui/params/param_widget.hpp>

#include<QPointer>
#include<ramen/ui/widgets/line_edit_fwd.hpp>

#include<ramen/nodes/image/input/image_seq_param.hpp>

class QPushButton;

namespace ramen
{
namespace ui
{

class image_seq_param_widget_t : public param_widget_t
{
    Q_OBJECT

public:

    explicit image_seq_param_widget_t( image_seq_param_t& p);

private:

    QWidget *do_create_widgets() override;
    void do_update_widgets() override;
    void do_enable_widgets( bool e) override;

    image_seq_param_t& param_;

    QPointer<ui::line_edit_t> input_;
    QPointer<QPushButton> button_;

private Q_SLOTS:

    void select_pushed();
};

} // ui
} // ramen

#endif
