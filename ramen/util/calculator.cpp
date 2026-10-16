// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/util/calculator.hpp>

#include<cctype>
#include<locale>
#include<sstream>

namespace ramen
{
namespace util
{

// Recursive descent parser for the grammar:
//
//  expression = term (('+' | '-') term)*
//  term       = factor (('*' | '/') factor)*
//  factor     = number | '(' expression ')' | '-' factor | '+' factor
struct calculator_t::impl
{
	std::optional<double> parse_and_eval( const std::string& s) const
	{
        parser_t p( s);

        double result;
        if( p.expression( result) && p.at_end())
            return result;

        return std::optional<double>();
	}

private:

    class parser_t
    {
    public:

        explicit parser_t( const std::string& s) : str_( s), pos_( 0) {}

        bool at_end()
        {
            skip_spaces();
            return pos_ == str_.size();
        }

        bool expression( double& result)
        {
            if( !term( result))
                return false;

            while( true)
            {
                char op = peek();

                if( op != '+' && op != '-')
                    return true;

                ++pos_;

                double rhs;
                if( !term( rhs))
                    return false;

                if( op == '+')
                    result += rhs;
                else
                    result -= rhs;
            }
        }

    private:

        bool term( double& result)
        {
            if( !factor( result))
                return false;

            while( true)
            {
                char op = peek();

                if( op != '*' && op != '/')
                    return true;

                ++pos_;

                double rhs;
                if( !factor( rhs))
                    return false;

                if( op == '*')
                    result *= rhs;
                else
                    result /= rhs;
            }
        }

        bool factor( double& result)
        {
            if( number( result))
                return true;

            char c = peek();

            if( c == '(')
            {
                ++pos_;

                if( !expression( result) || peek() != ')')
                    return false;

                ++pos_;
                return true;
            }

            if( c == '-' || c == '+')
            {
                ++pos_;

                if( !factor( result))
                    return false;

                if( c == '-')
                    result = -result;

                return true;
            }

            return false;
        }

        bool number( double& result)
        {
            skip_spaces();

            std::size_t start = pos_;
            std::size_t i = pos_;

            if( i < str_.size() && ( str_[i] == '+' || str_[i] == '-'))
                ++i;

            std::size_t digits = 0;

            while( i < str_.size() && std::isdigit( static_cast<unsigned char>( str_[i])))
            {
                ++i;
                ++digits;
            }

            if( i < str_.size() && str_[i] == '.')
            {
                ++i;

                while( i < str_.size() && std::isdigit( static_cast<unsigned char>( str_[i])))
                {
                    ++i;
                    ++digits;
                }
            }

            if( digits == 0)
                return false;

            // exponent
            if( i < str_.size() && ( str_[i] == 'e' || str_[i] == 'E'))
            {
                std::size_t j = i + 1;

                if( j < str_.size() && ( str_[j] == '+' || str_[j] == '-'))
                    ++j;

                if( j < str_.size() && std::isdigit( static_cast<unsigned char>( str_[j])))
                {
                    while( j < str_.size() && std::isdigit( static_cast<unsigned char>( str_[j])))
                        ++j;

                    i = j;
                }
            }

            std::istringstream is( str_.substr( start, i - start));
            is.imbue( std::locale::classic());

            if( !( is >> result))
                return false;

            pos_ = i;
            return true;
        }

        char peek()
        {
            skip_spaces();
            return pos_ < str_.size() ? str_[pos_] : 0;
        }

        void skip_spaces()
        {
            while( pos_ < str_.size() && std::isspace( static_cast<unsigned char>( str_[pos_])))
                ++pos_;
        }

        const std::string& str_;
        std::size_t pos_;
    };
};

calculator_t::calculator_t()	{ pimpl_ = new impl();}
calculator_t::~calculator_t()	{ delete pimpl_;}

std::optional<double> calculator_t::operator()( const std::string& s) const
{
	return pimpl_->parse_and_eval( s);
}

} // util
} // ramen
