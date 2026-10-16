// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PARAM_VALUE_CONCEPT_HPP
#define RAMEN_PARAM_VALUE_CONCEPT_HPP

#include<ramen/core/empty.hpp>

#include<ramen/Concepts/RegularConcept.hpp>

#include<concepts>

namespace ramen
{

/**
\ingroup concepts
\brief concept for param values
*/
template<class T>
concept ParamValueConcept = RegularConcept<T> && std::default_initializable<T> && std::equality_comparable<T>;

/**
\ingroup concepts
\brief concept for param values that have operator[] defined
*/
template<class T>
concept IndexableParamValueConcept = ParamValueConcept<T> && ( std::same_as<T, core::empty_t> || requires( const T& x) { x[0];});

} // ramen

#endif
