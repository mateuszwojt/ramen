// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_OCIO_COLORSPACE_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_OCIO_COLORSPACE_PARAM_WIDGET_HPP

#include<ramen/ui/params/param_widget.hpp>

#include<QPointer>
#include<ramen/ui/widgets/ocio_colorspace_combo_fwd.hpp>

#include<ramen/params/ocio_colorspace_param.hpp>

namespace ramen
{
namespace ui
{

class ocio_colorspace_param_widget_t : public param_widget_t
{
    Q_OBJECT

public:

    explicit ocio_colorspace_param_widget_t( ocio_colorspace_param_t& p);

private:

    QWidget *do_create_widgets() override;
    void do_enable_widgets( bool e) override;
    void do_update_widgets() override;

    ocio_colorspace_param_t& param_;

    QPointer<ui::ocio_colorspace_combo_t> menu_;

private Q_SLOTS:

    void colorspace_picked( const std::string& cs);
};

} // ui
} // ramen

#endif
