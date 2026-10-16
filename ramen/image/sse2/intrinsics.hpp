// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_IMAGE_SSE2_INTRINSICS_HPP
#define RAMEN_IMAGE_SSE2_INTRINSICS_HPP

#if defined( __aarch64__) || defined( _M_ARM64)
    // SSE intrinsics implemented on top of NEON.
    #include<sse2neon.h>
#else
    #include<emmintrin.h>
#endif

#endif
