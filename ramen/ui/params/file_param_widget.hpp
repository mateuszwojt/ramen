// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_FILE_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_FILE_PARAM_WIDGET_HPP

#include<ramen/ui/params/param_widget.hpp>

#include<QPointer>
#include<ramen/ui/widgets/line_edit_fwd.hpp>

#include<ramen/params/file_param.hpp>

class QPushButton;

namespace ramen
{
namespace ui
{

class file_param_widget_t : public param_widget_t
{
    Q_OBJECT

public:

    explicit file_param_widget_t( file_param_t& p);

private:

    void do_update_widgets() override;
    void do_enable_widgets( bool e) override;
    QWidget *do_create_widgets() override;

    void update_input_text( const std::filesystem::path& p);
    void update_input_text();

    file_param_t& param_;

    QPointer<ui::line_edit_t> input_;
    QPointer<QPushButton> button_;

private Q_SLOTS:

    void select_pushed();
    void text_changed();
};

} // ui
} // ramen

#endif
