// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_STRING_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_STRING_PARAM_WIDGET_HPP

#include<ramen/ui/params/param_widget.hpp>

#include<QPointer>
#include<ramen/ui/widgets/line_edit_fwd.hpp>
#include<ramen/ui/widgets/text_edit_fwd.hpp>

#include<ramen/params/string_param.hpp>

namespace ramen
{
namespace ui
{

class string_param_widget_t : public param_widget_t
{
    Q_OBJECT

public:

    explicit string_param_widget_t( string_param_t& p);

private:

    QWidget *do_create_widgets() override;
    void do_enable_widgets( bool e) override;
    void do_update_widgets() override;

    string_param_t& param_;

    QPointer<ui::line_edit_t> input_;
    QPointer<ui::text_edit_t> multi_input_;

private Q_SLOTS:

    void text_changed();
};

} // ui
} // ramen

#endif
