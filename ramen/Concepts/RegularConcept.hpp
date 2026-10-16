// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_REGULAR_CONCEPT_HPP
#define RAMEN_REGULAR_CONCEPT_HPP

#include<concepts>

namespace ramen
{

// Copy constructible and assignable. std::copyable is too strict for
// Imath types, their assignment operators return const references.
template <class T>
concept RegularConcept = std::copy_constructible<T> && requires( T& a, const T& b) { a = b;};

} // ramen

#endif
