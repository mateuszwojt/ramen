// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_COMBO_GROUP_PARAM_HPP
#define	RAMEN_COMBO_GROUP_PARAM_HPP

#include<ramen/params/composite_param.hpp>

namespace ramen
{

namespace ui
{
class combo_group_param_widget_t;
} // ui

class RAMEN_API combo_group_param_t : public composite_param_t
{
    friend class ui::combo_group_param_widget_t;

public:

    explicit combo_group_param_t( const std::string& name);

    void set_default_value( int x);

    void set_value( int x, change_reason reason = user_edited);

protected:

    combo_group_param_t( const combo_group_param_t& other);
    void operator=( const combo_group_param_t& other);

private:

    // the widget disables the whole group, keep the children state.
    virtual void do_enable_widgets( bool e) {}

    virtual param_t *do_clone() const { return new combo_group_param_t( *this);}

    virtual void do_add_to_hash( hash::generator_t& hash_gen) const;

    virtual std::unique_ptr<undo::command_t> do_create_command();

    virtual void do_read( const serialization::yaml_node_t& node);
    virtual void do_write( serialization::yaml_oarchive_t& out) const;

};

} // namespace

#endif
