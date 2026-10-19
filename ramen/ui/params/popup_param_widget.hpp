// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_POPUP_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_POPUP_PARAM_WIDGET_HPP

#include<ramen/ui/params/param_widget.hpp>

#include<QPointer>

#include<ramen/params/popup_param.hpp>

class QComboBox;

namespace ramen
{
namespace ui
{

class popup_param_widget_t : public param_widget_t
{
    Q_OBJECT

public:

    explicit popup_param_widget_t( popup_param_t& p);

private:

    QWidget *do_create_widgets() override;
    void do_enable_widgets( bool e) override;
    void do_update_widgets() override;

    popup_param_t& param_;

    QPointer<QComboBox> menu_;

private Q_SLOTS:

    void item_picked( int index);
};

} // ui
} // ramen

#endif
