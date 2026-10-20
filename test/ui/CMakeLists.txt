######################################################################
# Copyright (C) 2013 Esteban Tovagliari. All Rights Reserved.        #
# Licensed under the terms of the CDDL License.                      #
# See CDDL_LICENSE.txt for a copy of the license.                    #
######################################################################

# saves the viewer contents, for checking the user interface drawing.
add_executable( viewer_capture viewer_capture.cpp)
target_link_libraries( viewer_capture PRIVATE "$<LINK_LIBRARY:WHOLE_ARCHIVE,ramen>")

add_custom_command( TARGET viewer_capture POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/ocio ${PROJECT_BINARY_DIR}/ocio
                    VERBATIM)
