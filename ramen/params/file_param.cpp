// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/file_param.hpp>

#include<filesystem>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/params/param_set.hpp>

namespace ramen
{

file_param_t::file_param_t( const std::string& name) : static_param_t( name)
{
    is_input_ = true;
    set_default_value( std::filesystem::path());
    ext_list_string_ = "File (*.*)";
}

file_param_t::file_param_t( const file_param_t& other) : static_param_t( other)
{
    is_input_ = other.is_input_;
    ext_list_string_ = other.ext_list_string_;
}

void file_param_t::set_default_value( const std::filesystem::path& p) { value().assign( p);}

void file_param_t::set_value( const std::filesystem::path& p, change_reason reason)
{
    if( can_undo())
        param_set()->add_command( this);

    value().assign( p);
    emit_param_changed( reason);
}

bool file_param_t::empty() const
{
    std::filesystem::path p( get_value<std::filesystem::path>( *this));
    return p.empty();
}

bool file_param_t::file_exists() const
{
    std::filesystem::path p( get_value<std::filesystem::path>( *this));

    if( !p.empty())
        return std::filesystem::exists( p);

    return false;
}

std::string file_param_t::extension() const
{
    std::filesystem::path p( get_value<std::filesystem::path>( *this));
    return p.extension().string();
}

void file_param_t::set_extension( const std::string& ext)
{
    RAMEN_ASSERT( !is_input_);
    std::filesystem::path p( get_value<std::filesystem::path>( *this));
    p.replace_extension( ext);
    value().assign( p);
    update_widgets();
}

void file_param_t::do_add_to_hash( hash::generator_t& hash_gen) const
{
    if( is_input_)
        hash_gen << filesystem::hash_string( get_value<std::filesystem::path>( *this));
}

void file_param_t::do_read( const serialization::yaml_node_t& node)
{
    std::string val;
    node.get_value<std::string>( "value", val);
    set_value( filesystem::path_from_utf8( val), silent_edit);
}

void file_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
    out << YAML::Key << "value"
            << YAML::Value << YAML::DoubleQuoted
            << filesystem::file_string( get_value<std::filesystem::path>( *this));
}

void file_param_t::do_convert_relative_paths( const std::filesystem::path& old_base, const std::filesystem::path& new_base)
{
    std::filesystem::path p( get_value<std::filesystem::path>( *this));

    if( p.is_relative())
    {
        value().assign( filesystem::convert_relative_path( p, old_base, new_base));
        update_widgets();
    }
}

void file_param_t::do_make_paths_absolute()
{
    RAMEN_ASSERT( composition());

    std::filesystem::path p( get_value<std::filesystem::path>( *this));

    if( p.is_relative())
    {
        value().assign( composition()->relative_to_absolute( p));
        update_widgets();
    }
}

void file_param_t::do_make_paths_relative()
{
    RAMEN_ASSERT( composition());

    std::filesystem::path p( get_value<std::filesystem::path>( *this));

    if( p.is_absolute())
    {
        value().assign( composition()->absolute_to_relative( p));
        update_widgets();
    }
}

} // namespace
