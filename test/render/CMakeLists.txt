######################################################################
# Copyright (C) 2013 Esteban Tovagliari. All Rights Reserved.        #
# Licensed under the terms of the CDDL License.                      #
# See CDDL_LICENSE.txt for a copy of the license.                    #
######################################################################

add_executable( exr_compare exr_compare.cpp)
target_link_libraries( exr_compare PRIVATE OpenEXR::OpenEXR)

# largest difference allowed between a render and its reference image.
# Different compilers and CPUs don't produce bit identical floating point results.
set( RAMEN_RENDER_TEST_TOLERANCE "0" CACHE STRING "Render tests pixel value tolerance")

# renders frames first to last of a composition from the command line and
# compares the images with the ones in reference/name.
function( add_ramen_render_test name first last)
    set( OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/${name})
    configure_file( ${name}.rmn.in ${CMAKE_CURRENT_BINARY_DIR}/${name}.rmn @ONLY)

    add_test( NAME render_${name}
              COMMAND ${CMAKE_COMMAND}  -DRAMEN=$<TARGET_FILE:ramen.bin>
                                        -DEXR_COMPARE=$<TARGET_FILE:exr_compare>
                                        -DCOMP=${CMAKE_CURRENT_BINARY_DIR}/${name}.rmn
                                        -DFIRST=${first}
                                        -DLAST=${last}
                                        -DOUTPUT_DIR=${OUTPUT_DIR}
                                        -DREFERENCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/reference/${name}
                                        -DTOLERANCE=${RAMEN_RENDER_TEST_TOLERANCE}
                                        -P ${CMAKE_CURRENT_SOURCE_DIR}/render_test.cmake)

    set_tests_properties( render_${name} PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endfunction()

add_ramen_render_test( color_bars_blur 1 1)

# about 35 nodes in 9 output chains.
add_ramen_render_test( regression 1 2)
