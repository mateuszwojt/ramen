// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/image_seq_param_widget.hpp>

#include<ramen/nodes/image/input/image_seq_param.hpp>
#include<filesystem>

#include<QPushButton>
#include<QGridLayout>
#include<QHBoxLayout>
#include<QCheckBox>
#include<QLabel>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/inspector/inspector.hpp>
#include<ramen/ui/widgets/line_edit.hpp>

#include<ramen/app/application.hpp>
#include<ramen/params/param_set.hpp>
#include<ramen/params/parameterised.hpp>

namespace ramen
{
namespace ui
{

image_seq_param_widget_t::image_seq_param_widget_t( image_seq_param_t& p) : param_widget_t( p), param_( p) {}

QWidget *image_seq_param_widget_t::do_create_widgets()
{
    QWidget *top = new QWidget();
    QLabel *label = new QLabel( top);
    input_ = new ui::line_edit_t( top);
    button_ = new QPushButton( top);

    QSize s = input_->sizeHint();

    label->move( 0, 0);
    label->resize( app().ui()->inspector().left_margin() - 5, s.height());
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
    label->setText( param_.name().c_str());
	label->setToolTip( param_.id().c_str());
	
    button_->move( app().ui()->inspector().width() - s.height() - 10, 0);
    button_->resize( s.height(), s.height());
    button_->setText( "...");
    button_->setEnabled( param_.enabled());
    connect( button_, SIGNAL( clicked()), this, SLOT( select_pushed()));

    input_->move( app().ui()->inspector().left_margin(), 0);
    input_->resize( app().ui()->inspector().width() - app().ui()->inspector().left_margin()
                    - button_->width() - 10, s.height());

	input_->setReadOnly( true);
    input_->setEnabled( param_.enabled());
	input_->setText( QString::fromStdString( param_.input_text_));

    top->setMinimumSize( app().ui()->inspector().width(), s.height());
    top->setMaximumSize( app().ui()->inspector().width(), s.height());
    top->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    return top;
}

void image_seq_param_widget_t::do_enable_widgets( bool e)
{
    if( input_)
    {
        input_->setEnabled( e);
        button_->setEnabled( e);
    }
}

void image_seq_param_widget_t::select_pushed()
{
	std::filesystem::path p;
	bool sequence, relative;
	
	if( app().ui()->image_sequence_file_selector( p, sequence, relative))
		param_.file_picked( p, param_.proxy_level_, sequence, relative);
}

void image_seq_param_widget_t::do_update_widgets()
{
	if( input_)
		input_->setText( QString::fromStdString( param_.input_text_));
}

} // ui
} // ramen
