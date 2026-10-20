// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PROPORTIONAL_PARAM_HPP
#define	RAMEN_PROPORTIONAL_PARAM_HPP

#include<ramen/params/numeric_param.hpp>

#include<Imath/ImathVec.h>


namespace ramen
{

class RAMEN_API proportional_param_t : public numeric_param_t
{

public:

    explicit proportional_param_t( const std::string& name);

    bool proportional() const	    { return flags() & proportional_bit;}
    void set_proportional( bool p);

    /// True if proportional editing is enabled in the user interface.
    bool proportional_checked() const           { return proportional_checked_;}
    void set_proportional_checked( bool b)      { proportional_checked_ = b;}

protected:

    proportional_param_t( const proportional_param_t& other);
    void operator=( const proportional_param_t& other);

    static Imath::V3f proportional_factor;

private:

    bool proportional_checked_;
};

} // namespace

#endif
