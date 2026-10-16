// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/add_node_command.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/nodes/graph_algorithm.hpp>

namespace ramen
{
namespace undo
{

add_node_command_t::add_node_command_t( std::unique_ptr<node_t> n, node_t *src) : command_t( "Add Node"), storage_( std::move( n)), src_( src)
{
    node_ = storage_.get();
}

add_node_command_t::~add_node_command_t() {}

void add_node_command_t::undo()
{
    if( src_)
        app().document().composition().disconnect( src_, node_, 0);
	
    breadth_first_outputs_search( *node_, std::bind( &node_t::notify, std::placeholders::_1));
    storage_ = app().document().composition().release_node( node_);
    command_t::undo();
}

void add_node_command_t::redo()
{
    app().document().composition().add_node( std::move( storage_));

    if( src_)
	{
        app().document().composition().connect( src_, node_, 0);
		breadth_first_outputs_search( *node_, std::bind( &node_t::notify, std::placeholders::_1));
	}

    command_t::redo();
}

add_nodes_command_t::add_nodes_command_t() : command_t( "Add Nodes") {}

void add_nodes_command_t::add_node( std::unique_ptr<node_t> n)
{
    nodes_.push_back( n.get());
    node_storage_.push_back( std::move( n));
}

void add_nodes_command_t::undo()
{
    for( std::vector<node_t*>::const_iterator it( nodes_.begin()); it != nodes_.end(); ++it)
    {
        std::unique_ptr<node_t> ptr( app().document().composition().release_node( *it));
        node_storage_.push_back( std::move( ptr));
    }

    command_t::undo();
}

void add_nodes_command_t::redo()
{
    while( !node_storage_.empty())
    {
        std::unique_ptr<node_t> ptr( node_storage_.pop_back().release());
        app().document().composition().add_node( std::move( ptr));
    }

    command_t::redo();
}

} // namespace
} // namespace
