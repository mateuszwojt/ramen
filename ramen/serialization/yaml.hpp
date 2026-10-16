// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_YAML_HPP
#define RAMEN_YAML_HPP

#include<ramen/config.hpp>

#include<string>
#include<exception>

#include<boost/filesystem/fstream.hpp>

#include<yaml-cpp/yaml.h>

#include<Imath/ImathVec.h>
#include<Imath/ImathColor.h>

#include<ramen/core/empty.hpp>

#include<ramen/filesystem/path.hpp>

namespace YAML
{

// stream style extraction, as in the old yaml-cpp api.
template<class T>
void operator>>( const Node& in, T& value)
{
	value = in.as<T>();
}

} // YAML

namespace ramen
{

void check_yaml_errors( const YAML::Emitter& out);

// Returns the value for key, or an undefined node if it does not exist.
YAML::Node find_value( const YAML::Node& node, const std::string& key);

// utils
template<class T>
void get_value( const YAML::Node& node, const std::string& key, T& value)
{
	node[key] >> value;
}

template<class T>
bool get_optional_value( const YAML::Node& node, const std::string& key, T& value)
{
	if( const YAML::Node n = find_value( node, key))
	{
		try
		{
			n >> value;
			return true;
		}
		catch( YAML::Exception& e) {}
	}

	return false;
}

// nodes
YAML::Node get_node( const YAML::Node& node, const std::string& key);

// YAML overloads for common types

void operator>>( const YAML::Node& in, core::empty_t& x);
YAML::Emitter& operator<<( YAML::Emitter& out, const core::empty_t& x);

void operator>>( const YAML::Node& in, boost::filesystem::path& p);
YAML::Emitter& operator<<( YAML::Emitter& out, const boost::filesystem::path& p);

template<class T>
void operator>>( const YAML::Node& in, Imath::Vec2<T>& v)
{
	in[0] >> v.x;
	in[1] >> v.y;
}

template<class T>
YAML::Emitter& operator<<( YAML::Emitter& out, const Imath::Vec2<T>& v)
{
    out << YAML::Flow << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
    return out;
}

template<class T>
void operator>>( const YAML::Node& in, Imath::Vec3<T>& v)
{
	in[0] >> v.x;
	in[1] >> v.y;
	in[2] >> v.z;
}

template<class T>
YAML::Emitter& operator<<( YAML::Emitter& out, const Imath::Vec3<T>& v)
{
    out << YAML::Flow << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
    return out;
}

template<class T>
void operator>>( const YAML::Node& in, Imath::Color3<T>& c)
{
	in[0] >> c.x;
	in[1] >> c.y;
	in[2] >> c.z;
}

template<class T>
YAML::Emitter& operator<<( YAML::Emitter& out, const Imath::Color3<T>& c)
{
    out << YAML::Flow << YAML::BeginSeq << c.x << c.y << c.z << YAML::EndSeq;
    return out;
}

template<class T>
void operator>>( const YAML::Node& in, Imath::Color4<T>& c)
{
	in[0] >> c.r;
	in[1] >> c.g;
	in[2] >> c.b;
	in[3] >> c.a;
}

template<class T>
YAML::Emitter& operator<<( YAML::Emitter& out, const Imath::Color4<T>& c)
{
    out << YAML::Flow << YAML::BeginSeq << c.r << c.g << c.b << c.a << YAML::EndSeq;
    return out;
}

} // ramen

#endif
