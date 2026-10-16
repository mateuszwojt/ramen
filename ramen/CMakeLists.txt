######################################################################
# Copyright (C) 2013 Esteban Tovagliari. All Rights Reserved.        #
# Licensed under the terms of the CDDL License.                      #
# See CDDL_LICENSE.txt for a copy of the license.                    #
######################################################################

######################################################
# ramen lib sources

configure_file( ${PROJECT_SOURCE_DIR}/ramen/version.hpp.in
                ${PROJECT_BINARY_DIR}/include/ramen/version.hpp)

file( GLOB_RECURSE RAMEN_HEADERS CONFIGURE_DEPENDS *.h *.hpp)
file( GLOB_RECURSE RAMEN_SOURCES CONFIGURE_DEPENDS *.cpp *.c)

# the app entry point and code that is not used.
list( REMOVE_ITEM RAMEN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/ggems/fitcurves.c
                                ${CMAKE_CURRENT_SOURCE_DIR}/random/lookup3.c
                                )

######################################################
# ramen lib

add_library( ramen SHARED   ${RAMEN_HEADERS}
                            ${RAMEN_SOURCES}
                            ${PROJECT_SOURCE_DIR}/resources/resources.qrc
                            )

set_target_properties( ramen PROPERTIES
                             VERSION ${PROJECT_VERSION}
                             SOVERSION ${PROJECT_VERSION_MAJOR})

target_include_directories( ramen PUBLIC    ${PROJECT_SOURCE_DIR}
                                            ${PROJECT_SOURCE_DIR}/ramen
                                            ${PROJECT_BINARY_DIR}/include
                                            )

target_compile_definitions( ramen PUBLIC    BOOST_FILESYSTEM_NO_DEPRECATED
                                            QT_NO_KEYWORDS
                                            )

if( APPLE)
    target_compile_definitions( ramen PUBLIC GL_SILENCE_DEPRECATION)
endif()

target_link_libraries( ramen PUBLIC Qt6::Core
                                    Qt6::Gui
                                    Qt6::Widgets
                                    Qt6::OpenGL
                                    Qt6::OpenGLWidgets

                                    OpenColorIO::OpenColorIO
                                    OpenImageIO::OpenImageIO

                                    Imath::Imath
                                    OpenEXR::OpenEXR

                                    JPEG::JPEG
                                    PNG::PNG
                                    TIFF::TIFF

                                    GLEW::GLEW
                                    OpenGL::GL

                                    Boost::filesystem
                                    Boost::thread
                                    TBB::tbb
                                    yaml-cpp::yaml-cpp
                                    Agg::Agg
                                    ZLIB::ZLIB

                                    Threads::Threads
                                    )

if( WIN32)
    target_link_libraries( ramen PUBLIC Userenv)
endif()

if( SSE2NEON_INCLUDE_DIR)
    target_include_directories( ramen PRIVATE ${SSE2NEON_INCLUDE_DIR})
endif()

######################################################
# data files

add_custom_command( TARGET ramen POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/ocio ${PROJECT_BINARY_DIR}/ocio
                    VERBATIM)

######################################################
# ramen binary

add_executable( ramen.bin main.cpp)
target_link_libraries( ramen.bin PRIVATE ramen)
set_target_properties( ramen.bin PROPERTIES OUTPUT_NAME ramen)
