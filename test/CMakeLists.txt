######################################################################
# Copyright (C) 2013 Esteban Tovagliari. All Rights Reserved.        #
# Licensed under the terms of the CDDL License.                      #
# See CDDL_LICENSE.txt for a copy of the license.                    #
######################################################################

function( add_ramen_test sourcefile)
    add_executable( ${sourcefile} ${sourcefile}.cpp)
    target_link_libraries( ${sourcefile} PRIVATE ramen GTest::gmock)
    add_test( NAME ${sourcefile} COMMAND ${sourcefile})
endfunction()

add_subdirectory( color)
add_subdirectory( container)
add_subdirectory( core)
add_subdirectory( filesystem)
add_subdirectory( math)
add_subdirectory( util)
add_subdirectory( render)
add_subdirectory( ui)
