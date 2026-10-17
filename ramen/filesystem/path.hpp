// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_FILESYSTEM_PATH_HPP
#define RAMEN_FILESYSTEM_PATH_HPP

#include<filesystem>

namespace ramen
{
namespace filesystem
{

/// Returns the path as an UTF-8 string.
std::string file_string( const std::filesystem::path& p);

/// Makes a path from an UTF-8 string.
std::filesystem::path path_from_utf8( const std::string& s);

std::string hash_string( const std::filesystem::path& p);

std::filesystem::path make_absolute_path( const std::filesystem::path& p,
                                            const std::filesystem::path& from);

std::filesystem::path make_relative_path( const std::filesystem::path& p,
                                            const std::filesystem::path& from);

std::filesystem::path convert_relative_path( const std::filesystem::path& p,
                                               const std::filesystem::path& old_base,
                                                const std::filesystem::path& new_base);

} // filesystem
} // ramen

#endif
