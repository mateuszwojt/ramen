######################################################################
# Copyright (C) 2013 Esteban Tovagliari. All Rights Reserved.        #
# Licensed under the terms of the CDDL License.                      #
# See CDDL_LICENSE.txt for a copy of the license.                    #
######################################################################

# renders a composition from the command line and checks that the image was written.
function( add_ramen_render_test name)
    configure_file( ${name}.rmn.in ${CMAKE_CURRENT_BINARY_DIR}/${name}.rmn @ONLY)

    add_test( NAME render_${name}
              COMMAND ${CMAKE_COMMAND}  -DRAMEN=$<TARGET_FILE:ramen.bin>
                                        -DCOMP=${CMAKE_CURRENT_BINARY_DIR}/${name}.rmn
                                        -DOUTPUT=${CMAKE_CURRENT_BINARY_DIR}/${name}.0001.exr
                                        -P ${CMAKE_CURRENT_SOURCE_DIR}/render_test.cmake)

    set_tests_properties( render_${name} PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endfunction()

add_ramen_render_test( color_bars_blur)
