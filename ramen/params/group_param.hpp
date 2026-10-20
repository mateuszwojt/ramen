// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_GROUP_PARAM_HPP
#define	RAMEN_GROUP_PARAM_HPP

#include<ramen/params/composite_param.hpp>

namespace ramen
{

namespace ui
{
class group_param_widget_t;
} // ui

class RAMEN_API group_param_t : public composite_param_t
{
    friend class ui::group_param_widget_t;

public:

    explicit group_param_t( const std::string& name);

    bool can_collapse() const		{ return can_collapse_;}
    void set_can_collapse( bool b)	{ can_collapse_ = b;}

protected:

    group_param_t( const group_param_t& other);
    void operator=( const group_param_t& other);

private:

    // the widget disables the whole group, keep the children state.
    virtual void do_enable_widgets( bool e) {}

    virtual param_t *do_clone() const { return new group_param_t( *this);}

    bool can_collapse_;
};

} // ramen

#endif
