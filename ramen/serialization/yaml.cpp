// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/serialization/yaml.hpp>

#include<ramen/assert.hpp>

#ifndef NDEBUG
	#include<iostream>
#endif

namespace ramen
{

void check_yaml_errors( const YAML::Emitter& out)
{
	#ifndef NDEBUG
		if( !out.good())
		{
			std::cout << "YAML error " << out.GetLastError() << "\n";
			RAMEN_ASSERT( 0);
		}
	#endif
}

YAML::Node find_value( const YAML::Node& node, const std::string& key)
{
	if( node.IsMap())
		return node[key];

	return YAML::Node( YAML::NodeType::Undefined);
}

YAML::Node get_node( const YAML::Node& node, const std::string& key)
{
	return node[key];
}

void operator>>( const YAML::Node& in, core::empty_t& x)
{
    RAMEN_ASSERT( 0 && "Trying to read an core::empty_t from a YAML::Node");
}

YAML::Emitter& operator<<( YAML::Emitter& out, const core::empty_t& x)
{
    RAMEN_ASSERT( 0 && "Trying to write an core::empty_t to a YAML::Emitter");
    return out;
}

void operator>>( const YAML::Node& in, std::filesystem::path& p)
{
	std::string tmp;
	in >> tmp;
	p = filesystem::path_from_utf8( tmp);
}

YAML::Emitter& operator<<( YAML::Emitter& out, const std::filesystem::path& p)
{
    out << YAML::DoubleQuoted << filesystem::file_string( p);
    return out;
}

} // ramen
