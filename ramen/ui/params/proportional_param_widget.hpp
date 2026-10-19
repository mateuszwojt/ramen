// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_PROPORTIONAL_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_PROPORTIONAL_PARAM_WIDGET_HPP

#include<ramen/ui/params/param_widget.hpp>

#include<QPointer>
#include<QToolButton>

#include<ramen/params/proportional_param.hpp>

namespace ramen
{
namespace ui
{

/// Base class for the widgets of params that can be edited proportionally.
class proportional_param_widget_t : public param_widget_t
{
    Q_OBJECT

protected:

    explicit proportional_param_widget_t( proportional_param_t& p);

    void create_proportional_button( QWidget *parent, int height);

    QPointer<QToolButton> prop_button_;

private Q_SLOTS:

	void proportional_toggle( bool state);

private:

    proportional_param_t& prop_param_;
};

} // ui
} // ramen

#endif
