// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/manipulators/overlays.hpp>

#include<map>

#include<ramen/nodes/node.hpp>
#include<ramen/nodes/node_metaclass.hpp>

namespace ramen
{
namespace ui
{
namespace
{

std::map<std::string, overlay_creator_t>& creators()
{
    static std::map<std::string, overlay_creator_t> c;
    return c;
}

} // unnamed

void register_overlay( const std::string& node_class_id, const overlay_creator_t& creator)
{
    creators()[node_class_id] = creator;
}

std::unique_ptr<manipulable_t> create_overlay( node_t& n)
{
    if( !n.metaclass())
        return std::unique_ptr<manipulable_t>();

    std::map<std::string, overlay_creator_t>::const_iterator it( creators().find( n.metaclass()->id));

    if( it == creators().end())
        return std::unique_ptr<manipulable_t>();

    return it->second( n);
}

} // ui
} // ramen
