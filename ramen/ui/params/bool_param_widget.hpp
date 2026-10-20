// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_BOOL_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_BOOL_PARAM_WIDGET_HPP

#include<ramen/ui/params/param_widget.hpp>

#include<QPointer>
#include<QCheckBox>

#include<ramen/params/bool_param.hpp>

namespace ramen
{
namespace ui
{

class bool_param_widget_t : public param_widget_t
{
    Q_OBJECT

public:

    explicit bool_param_widget_t( bool_param_t& p);

private:

    void do_update_widgets() override;
    QWidget *do_create_widgets() override;
    void do_enable_widgets( bool e) override;

    bool_param_t& param_;

    QPointer<QCheckBox> button_;

private Q_SLOTS:

        void button_checked( int state);
};

} // ui
} // ramen

#endif
