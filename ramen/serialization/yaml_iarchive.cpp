// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/serialization/yaml_iarchive.hpp>

#include<vector>
#include<string>
#include<sstream>

#include<fstream>

namespace ramen
{
namespace serialization
{

yaml_iarchive_t::yaml_iarchive_t( std::istream& is)
{ 
	init( is);
}

void yaml_iarchive_t::init( std::istream& is)
{
	RAMEN_ASSERT( is.good());

	version_ = 0;
	header_read_ = false;

	doc_ = YAML::Load( is);
	root_.reset( new yaml_node_t( this, doc_, version_));
}

const yaml_node_t& yaml_iarchive_t::root() const
{ 
	RAMEN_ASSERT( root_.get());
	return *root_.get();
}

bool yaml_iarchive_t::read_composition_header()
{
	RAMEN_ASSERT( version_ == 0);
	RAMEN_ASSERT( !header_read_);

	std::string magic;

	if( !get_optional_value( "magic", magic))
		return false;

	if( magic != "Ramen composition")
		return false;

	if( !get_optional_value( "version", version_))
		return false;

	if( version() < 1)
		return false;

	root().set_version( version());
	header_read_ = true;
	return true;
}

yaml_node_t yaml_iarchive_t::get_node( const std::string& key) const
{ 
	return root().get_node( key);
}

std::optional<yaml_node_t> yaml_iarchive_t::get_optional_node( const std::string& key) const
{ 
	return root().get_optional_node( key);
}

std::string yaml_iarchive_t::errors() const
{
	return error_stream_.str();
}

std::ios_base::openmode yaml_iarchive_t::file_open_mode()
{
	return std::ios::in;
}

} // namespace
} // namespace
