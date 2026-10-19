// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/node_gui_creators.hpp>

#include<filesystem>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/nodes/node_factory.hpp>
#include<ramen/nodes/image/input/input_node.hpp>

#include<ramen/ui/user_interface.hpp>

namespace ramen
{
namespace ui
{
namespace
{

node_t *create_gui_image_input_node()
{
    bool sequence, relative;
    std::filesystem::path p;

    if( app().ui()->image_sequence_file_selector( p, sequence, relative))
        return new image::input_node_t( p, sequence, app().document().composition().composition_dir());

    return 0;
}

} // unnamed

void register_node_gui_creators()
{
    node_factory_t::instance().set_gui_creator( "image.builtin.image_input", &create_gui_image_input_node);
}

} // ui
} // ramen
