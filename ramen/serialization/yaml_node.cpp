// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/serialization/yaml_node.hpp>
#include<ramen/serialization/yaml_iarchive.hpp>

namespace ramen
{
namespace serialization
{

yaml_node_t::yaml_node_t( yaml_iarchive_t *parent, const YAML::Node& node, int version) : node_( node)
{
	RAMEN_ASSERT( parent);
	RAMEN_ASSERT( version >= 0);

	parent_ = parent;
	version_ = version;
}

yaml_node_t::yaml_node_t( const yaml_node_t& parent, const YAML::Node& node) : node_( node)
{
	parent_ = parent.parent_;
	version_ = parent.version_;
}

void yaml_node_t::set_version( int v) const
{
	RAMEN_ASSERT( v >= 0);
	version_ = v;
}

yaml_node_t yaml_node_t::get_node( const std::string& key) const
{
	return yaml_node_t( parent_, ramen::get_node( node_, key), version());
}

std::optional<yaml_node_t> yaml_node_t::get_optional_node( const std::string& key) const
{
	if( const YAML::Node n = find_value( node_, key))
		return yaml_node_t( parent_, n, version());
	
	return std::optional<yaml_node_t>();
}

std::stringstream& yaml_node_t::error_stream() const
{
	RAMEN_ASSERT( parent_);
	return parent_->error_stream();
}

} // namespace
} // namespace
