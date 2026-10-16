// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_FILESYSTEM_PATH_SEQUENCE_HPP
#define	RAMEN_FILESYSTEM_PATH_SEQUENCE_HPP

#include<tuple>
#include<filesystem>

#include<ramen/filesystem/path.hpp>

#include<ramen/serialization/yaml.hpp>

namespace ramen
{
namespace filesystem
{
	
class path_sequence_t
{
public:

    path_sequence_t();
    path_sequence_t( const std::filesystem::path& p, bool sequence = true);
	path_sequence_t( const std::string& format, int start, int end);

	void swap( path_sequence_t& other);
	
    bool valid() const { return valid_;}

    std::filesystem::path operator()() const;
    std::filesystem::path operator()( int n) const;

    std::string format_string() const;

    bool add_path( const std::filesystem::path& p);
	void add_paths( std::filesystem::directory_iterator it);
	
	const std::filesystem::path& directory() const		{ return dir_;}
	void set_directory( const std::filesystem::path& p)	{ dir_ = p;}
	
    bool is_sequence() const { return is_seq_;}

    int start() const   { return start_;}
    int end() const     { return end_;}
	int pad() const		{ return pad_;}

	// paths
	void convert_relative_paths( const std::filesystem::path& old_base, const std::filesystem::path& new_base);
	void make_paths_absolute( const std::filesystem::path& from_dir);
	void make_paths_relative( const std::filesystem::path& from_dir);

	// serialization
	friend void operator>>( const YAML::Node& in, path_sequence_t& seq);
	friend YAML::Emitter& operator<<( YAML::Emitter& out, const path_sequence_t& seq);

private:

    std::string get_extension( const std::string& s) const;
    std::string get_frame_number( const std::string& s) const;
    int get_pad( const std::string& str) const;
    std::tuple<std::string, std::string, std::string> decompose_filename( const std::string& filename) const;

	bool valid_;
    std::filesystem::path dir_;
    std::string base_;
    int start_, end_;
    int pad_;
    std::string ext_;
    bool is_seq_;
};

} // namespace
} // namespace

#endif
