// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_UTIL_CALCULATOR_HPP
#define RAMEN_UTIL_CALCULATOR_HPP

#include<string>

#include<ramen/core/noncopyable.hpp>
#include<optional>

namespace ramen
{
namespace util
{

class calculator_t : ramen::core::noncopyable_t
{
public:

	calculator_t();
	~calculator_t();

	std::optional<double> operator()( const std::string& str) const;
	
private:

	struct impl;
	impl *pimpl_;
};
	
} // util
} // ramen

#endif
