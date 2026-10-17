// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<string>
#include<vector>

#include<windows.h>
#include<lmcons.h>
#include<shlobj.h>

namespace ramen
{
namespace system
{
namespace
{

std::string utf8_from_wide( const wchar_t *str)
{
    int size = WideCharToMultiByte( CP_UTF8, 0, str, -1, 0, 0, 0, 0);

    if( size <= 1)
        return std::string();

    std::string result( size - 1, '\0');
    WideCharToMultiByte( CP_UTF8, 0, str, -1, &result[0], size, 0, 0);
    return result;
}

std::filesystem::path known_folder_path( REFKNOWNFOLDERID id)
{
    std::filesystem::path result;
    PWSTR path = 0;

    if( SHGetKnownFolderPath( id, 0, 0, &path) == S_OK)
        result = std::filesystem::path( path);

    CoTaskMemFree( path);
    return result;
}

} // unnamed

struct system_t::impl
{
    impl( system_t& self)
    {
        self.system_name_ = "Windows";

        // user name
        {
            wchar_t name[UNLEN + 1];
            DWORD size = UNLEN + 1;

            if( GetUserNameW( name, &size))
                self.user_name_ = utf8_from_wide( name);
        }

        self.home_path_ = known_folder_path( FOLDERID_Profile);

        // app user path
        {
            std::string dir_name( "ramen");
            dir_name.append( RAMEN_VERSION_MAJOR_STR);

            std::filesystem::path app_data( known_folder_path( FOLDERID_RoamingAppData));

            if( app_data.empty())
                app_data = self.home_path_;

            self.application_user_path_ = app_data / dir_name;
        }

        // ram size
        {
            MEMORYSTATUSEX status;
            status.dwLength = sizeof( status);

            if( GlobalMemoryStatusEx( &status))
                self.ram_size_ = status.ullTotalPhys;
        }

        // executable location
        {
            std::vector<wchar_t> buffer( MAX_PATH);

            while( true)
            {
                DWORD size = GetModuleFileNameW( 0, buffer.data(), static_cast<DWORD>( buffer.size()));

                if( size == 0)
                    break;

                if( size < buffer.size())
                {
                    self.executable_path_ = std::filesystem::path( std::wstring( buffer.data(), size));
                    break;
                }

                buffer.resize( buffer.size() * 2);
            }
        }
    }
};

} // system
} // ramen
