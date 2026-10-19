// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_ROTO_SHAPE_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_ROTO_SHAPE_PARAM_WIDGET_HPP

#include<ramen/ui/params/param_widget.hpp>

#include<QPointer>
#include<ramen/ui/widgets/line_edit_fwd.hpp>
#include<ramen/ui/widgets/color_button_fwd.hpp>

#include<ramen/nodes/image/roto/roto_shape_param.hpp>

class QComboBox;
class QToolButton;
class QPushButton;

namespace ramen
{
namespace ui
{

class roto_shape_param_widget_t : public param_widget_t
{
    Q_OBJECT

public:

    explicit roto_shape_param_widget_t( roto_shape_param_t& p);

private:

    void update_parent_menu();
    QWidget *do_create_widgets() override;
    void do_update_widgets() override;
    void show_shape( roto::shape_t *s);

    roto_shape_param_t& param_;

	QPointer<ui::line_edit_t> name_input_;
	QPointer<QComboBox> parent_menu_;
	QPointer<ui::color_button_t> display_color_;
	QPointer<QToolButton> autokey_;
	QPointer<QPushButton> shape_key_;
	QPointer<QToolButton> order_up_;
	QPointer<QToolButton> order_down_;

private Q_SLOTS:

    void rename_shape();	
	void set_autokey( bool b);
	void set_shape_key();
    void change_shape_color();
    void set_shape_parent( int index);
	void move_shape_order_up();
	void move_shape_order_down();
};

} // ui
} // ramen

#endif
