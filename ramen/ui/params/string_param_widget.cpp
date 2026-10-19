// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/string_param_widget.hpp>

#include<ramen/params/string_param.hpp>
#include<ramen/nodes/node.hpp>

#include<QLabel>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/inspector/inspector.hpp>
#include<ramen/ui/widgets/line_edit.hpp>
#include<ramen/ui/widgets/text_edit.hpp>

#include<ramen/app/application.hpp>
#include<ramen/params/param_set.hpp>
#include<ramen/params/parameterised.hpp>

namespace ramen
{
namespace ui
{

string_param_widget_t::string_param_widget_t( string_param_t& p) : param_widget_t( p), param_( p) {}

QWidget *string_param_widget_t::do_create_widgets()
{
    QWidget *top = new QWidget();
    QLabel *label = new QLabel( top);
    ui::line_edit_t *tmp = new ui::line_edit_t();
    QSize s = tmp->sizeHint();
    delete tmp;

    label->move( 0, 0);
    label->resize( app().ui()->inspector().left_margin() - 5, s.height());
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
    label->setText( param_.name().c_str());
    label->setToolTip( param_.id().c_str());

    std::string str = get_value<std::string>( param_);
    int height;

    if( param_.multiline())
    {
        multi_input_ = new ui::text_edit_t( top);
        multi_input_->setReadOnly( param_.read_only_);
        multi_input_->move( app().ui()->inspector().left_margin(), 0);
        multi_input_->resize( app().ui()->inspector().width() - app().ui()->inspector().left_margin() - 10, s.height() * 7);
        multi_input_->setEnabled( param_.enabled());
        multi_input_->setPlainText( str.c_str());
        connect( multi_input_, SIGNAL( textHasChanged()), this, SLOT( text_changed()));
        height = multi_input_->height();
    }
    else
    {
        input_ = new ui::line_edit_t( top);
        input_->setReadOnly( param_.read_only_);
        input_->move( app().ui()->inspector().left_margin(), 0);
        input_->resize( app().ui()->inspector().width() - app().ui()->inspector().left_margin() - 10, s.height());
        input_->setEnabled( param_.enabled());
        input_->setText( str.c_str());
        connect( input_, SIGNAL( editingFinished()), this, SLOT( text_changed()));
        height = s.height();
    }

    top->setMinimumSize( app().ui()->inspector().width(), height);
    top->setMaximumSize( app().ui()->inspector().width(), height);
    top->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    return top;
}

void string_param_widget_t::do_update_widgets()
{
    if( input_)
    {
        input_->blockSignals( true);
        std::string str = get_value<std::string>( param_);
        input_->setText( str.c_str());
        input_->blockSignals( false);
    }
    else
    {
        if( multi_input_)
        {
            multi_input_->blockSignals( true);
            std::string str = get_value<std::string>( param_);
            multi_input_->setPlainText( str.c_str());
            multi_input_->blockSignals( false);
        }
    }
}

void string_param_widget_t::do_enable_widgets( bool e)
{
    if( input_)
        input_->setEnabled( e);
    else
    {
        if( multi_input_)
            multi_input_->setEnabled( e);
    }
}

void string_param_widget_t::text_changed()
{
    std::string str;

    if( param_.multiline())
        str = multi_input_->toPlainText().toStdString();
    else
        str = input_->text().toStdString();

    param_.param_set()->begin_edit();
    param_.set_value( str);
    param_.param_set()->end_edit();
}

} // ui
} // ramen
