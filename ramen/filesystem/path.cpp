// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/filesystem/path.hpp>

#include<sstream>

#include<filesystem>

#include<QDir>

#include<ramen/assert.hpp>

namespace ramen
{
namespace filesystem
{

std::string file_string( const std::filesystem::path& p)
{
    std::u8string s( p.u8string());
    return std::string( s.begin(), s.end());
}

std::filesystem::path path_from_utf8( const std::string& s)
{
    return std::filesystem::path( std::u8string( s.begin(), s.end()));
}

std::string hash_string( const std::filesystem::path& p)
{
    std::stringstream s;
    s << p;

	if( std::filesystem::exists( p))
	{
		s << static_cast<long long>( std::filesystem::last_write_time( p).time_since_epoch().count());
	}
	
	return s.str();
}

std::filesystem::path make_absolute_path( const std::filesystem::path& p,
                                            const std::filesystem::path& from)
{
    RAMEN_ASSERT( p.is_relative());
    RAMEN_ASSERT( from.is_absolute());

    QDir dir( QString::fromStdString( file_string( from)));
    QString fname( QString::fromStdString( file_string( p)));
    QString abs_path( QDir::cleanPath( dir.absoluteFilePath( fname)));
    return path_from_utf8( abs_path.toStdString());
}

std::filesystem::path make_relative_path( const std::filesystem::path& p,
                                            const std::filesystem::path& from)
{
    RAMEN_ASSERT( p.is_absolute());
    RAMEN_ASSERT( from.is_absolute());

    QDir dir( QString::fromStdString( file_string( from)));
    QString fname( QString::fromStdString( file_string( p)));
    QString rel_path( dir.relativeFilePath( fname));
    return path_from_utf8( rel_path.toStdString());
}

std::filesystem::path convert_relative_path( const std::filesystem::path& p,
                                               const std::filesystem::path& old_base,
                                               const std::filesystem::path& new_base)
{
    std::filesystem::path p0( make_absolute_path( p, old_base));
    return make_relative_path( p0, new_base);
}

} // filesystem
} // ramen
