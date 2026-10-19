// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/params/file_param_widget.hpp>

#include<ramen/params/file_param.hpp>
#include<filesystem>
#include<ramen/app/document.hpp>

#include<QPushButton>
#include<QFileDialog>
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

file_param_widget_t::file_param_widget_t( file_param_t& p) : param_widget_t( p), param_( p) {}

void file_param_widget_t::do_update_widgets()
{
    if( input_)
        update_input_text();
}

void file_param_widget_t::do_enable_widgets( bool e)
{
    if( input_)
    {
        input_->setEnabled( e);
        button_->setEnabled( e);
    }
}

QWidget *file_param_widget_t::do_create_widgets()
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

    if( param_.is_input_)
        input_->setReadOnly( true);

    input_->setEnabled( param_.enabled());

    update_input_text();

    connect( input_, SIGNAL( editingFinished()), this, SLOT( text_changed()));

    top->setMinimumSize( app().ui()->inspector().width(), s.height());
    top->setMaximumSize( app().ui()->inspector().width(), s.height());
    top->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    return top;
}

void file_param_widget_t::select_pushed()
{
    QString fname;
    static bool was_relative = false;
    bool relative  = false;

    if( param_.is_input_)
    {
        QFileDialog dialog( 0, "Select file", QString(), param_.ext_list_string_.c_str());
        dialog.setOption( QFileDialog::DontUseNativeDialog, true);
        dialog.setFileMode( QFileDialog::ExistingFile);

        QCheckBox *relative_check = new QCheckBox( "Relative");

        if( app().document().has_file())
            relative_check->setChecked( was_relative);
        else
        {
            was_relative = false;
            relative_check->setChecked( false);
            relative_check->setEnabled( false);
        }

        QGridLayout *glayout = (QGridLayout *) dialog.layout();
        glayout->addWidget( relative_check, glayout->rowCount(), 0, 1, glayout->columnCount(), Qt::AlignLeft);

        dialog.show();

        if( dialog.exec())
        {
            QStringList filenames = dialog.selectedFiles();
            fname = filenames[0];
            relative = relative_check->isChecked();
            was_relative = relative;
        }
    }
    else
        fname = QFileDialog::getSaveFileName( 0, "Select file", QString(), param_.ext_list_string_.c_str(),
                                              0, QFileDialog::DontUseNativeDialog);

    if( !fname.isEmpty())
    {
        std::filesystem::path p( filesystem::path_from_utf8( fname.toStdString()));

        if( param_.is_input_ && relative)
        {
            RAMEN_ASSERT( param_.composition());
            p = param_.composition()->absolute_to_relative( p);
        }

        update_input_text( p);

        param_.param_set()->begin_edit();
        param_.set_value( p);
        param_.param_set()->end_edit();
    }
}

void file_param_widget_t::text_changed()
{
    std::filesystem::path p( filesystem::path_from_utf8( input_->text().toStdString()));
    param_.param_set()->begin_edit();
    param_.set_value( p);
    param_.param_set()->end_edit();
}

void file_param_widget_t::update_input_text( const std::filesystem::path& p)
{
    if( input_)
    {
        input_->blockSignals( true);
        input_->setText( filesystem::file_string( p).c_str());
        input_->blockSignals( false);
    }
}

void file_param_widget_t::update_input_text()
{
    update_input_text( get_value<std::filesystem::path>( param_));
}

} // ui
} // ramen
