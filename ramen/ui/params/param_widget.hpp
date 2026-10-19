// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PARAMS_PARAM_WIDGET_HPP
#define RAMEN_UI_PARAMS_PARAM_WIDGET_HPP

#include<ramen/config.hpp>

#include<functional>

#include<QObject>

#include<ramen/core/signal.hpp>
#include<ramen/params/param.hpp>

class QWidget;

namespace ramen
{
namespace ui
{

/*!
\ingroup ui
\brief Base class for the inspector widgets that edit a param.
The widget object is owned by the Qt widgets it creates.
*/
class RAMEN_API param_widget_t : public QObject
{
    Q_OBJECT

public:

    explicit param_widget_t( param_t& p);
    virtual ~param_widget_t();

    /// Creates the Qt widgets. Returns null if the param has no widgets.
    QWidget *create_widgets();

protected:

    virtual QWidget *do_create_widgets() = 0;
    virtual void do_update_widgets();
    virtual void do_enable_widgets( bool e);

private:

    core::connection_t update_connection_;
    core::connection_t enable_connection_;
};

typedef std::function<param_widget_t*( param_t&)> param_widget_creator_t;

/// Registers the widget class used for a param class and its subclasses.
RAMEN_API void register_param_widget( const param_widget_creator_t& creator);

template<class Param, class Widget>
void register_param_widget()
{
    register_param_widget( []( param_t& p) -> param_widget_t*
    {
        if( Param *x = dynamic_cast<Param*>( &p))
            return new Widget( *x);

        return 0;
    });
}

/// Registers the widgets for the builtin params.
RAMEN_API void register_builtin_param_widgets();

/// Creates the widgets for a param. Returns null if the param has no widgets.
RAMEN_API QWidget *create_param_widgets( param_t& p);

} // ui
} // ramen

#endif
