// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UTIL_LEXICAL_CAST_HPP
#define RAMEN_UTIL_LEXICAL_CAST_HPP

#include<limits>
#include<locale>
#include<sstream>
#include<stdexcept>
#include<string>
#include<type_traits>

namespace ramen
{
namespace util
{

struct bad_lexical_cast : public std::runtime_error
{
    bad_lexical_cast() : std::runtime_error( "bad lexical cast") {}
};

/// Converts between types using their stream operators. Throws bad_lexical_cast
/// if the whole source can't be converted.
template<class Target, class Source>
Target lexical_cast( const Source& x)
{
    std::stringstream s;
    s.imbue( std::locale::classic());

    if constexpr( std::is_floating_point_v<Source>)
        s.precision( std::numeric_limits<Source>::max_digits10);

    s << x;

    if constexpr( std::is_same_v<Target, std::string>)
        return s.str();
    else
    {
        Target result;
        s >> std::noskipws >> result;

        if( s.fail() || s.get() != std::char_traits<char>::eof())
            throw bad_lexical_cast();

        return result;
    }
}

} // util
} // ramen

#endif
