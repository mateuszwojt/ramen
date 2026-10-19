// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_BOOL_PARAM_HPP
#define	RAMEN_BOOL_PARAM_HPP

#include<ramen/params/static_param.hpp>

namespace ramen
{

namespace ui
{
class bool_param_widget_t;
} // ui

class RAMEN_API bool_param_t : public static_param_t
{
    friend class ui::bool_param_widget_t;

public:

    explicit bool_param_t( const std::string& name);

    void set_default_value( bool x);

    void set_value( bool x, change_reason reason = user_edited);

protected:

    bool_param_t( const bool_param_t& other);
    void operator=( const bool_param_t& other);

private:

    virtual param_t *do_clone() const;

    virtual void do_add_to_hash( hash::generator_t& hash_gen) const;

    virtual void do_read( const serialization::yaml_node_t& node);
    virtual void do_write( serialization::yaml_oarchive_t& out) const;

};

} // namespace

#endif
