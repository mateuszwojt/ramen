// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_TAB_GROUP_PARAM_HPP
#define	RAMEN_TAB_GROUP_PARAM_HPP

#include<ramen/params/composite_param.hpp>

namespace ramen
{

namespace ui
{
class tab_group_param_widget_t;
} // ui

class tab_group_param_t : public composite_param_t
{
    friend class ui::tab_group_param_widget_t;

public:

	tab_group_param_t();
    explicit tab_group_param_t( const std::string& name);

protected:

    tab_group_param_t( const tab_group_param_t& other);
    void operator=( const tab_group_param_t& other);

private:

    // the widget disables the whole group, keep the children state.
    virtual void do_enable_widgets( bool e) {}

    virtual param_t *do_clone() const { return new tab_group_param_t( *this);}

    virtual void do_create_tracks( anim::track_t *parent);

};

} // namespace

#endif
