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

# a static library, the executables link all of it ( see below).
add_library( ramen STATIC   ${RAMEN_HEADERS}
                            ${RAMEN_SOURCES}
                            ${PROJECT_SOURCE_DIR}/resources/resources.qrc
                            )

target_include_directories( ramen PUBLIC    ${PROJECT_SOURCE_DIR}
                                            ${PROJECT_SOURCE_DIR}/ramen
                                            ${PROJECT_BINARY_DIR}/include
                                            )

target_compile_definitions( ramen PUBLIC    QT_NO_KEYWORDS
                                            RAMEN_STATIC_LIB
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
                                    OpenGL::GLU

                                    Boost::headers
                                    TBB::tbb
                                    yaml-cpp::yaml-cpp
                                    Agg::Agg
                                    ZLIB::ZLIB

                                    Threads::Threads
                                    )

if( WIN32)
    target_link_libraries( ramen PUBLIC Shell32 Ole32)
endif()

if( SSE2NEON_INCLUDE_DIR)
    target_include_directories( ramen PRIVATE ${SSE2NEON_INCLUDE_DIR})
endif()

######################################################
# ramen binary

if( APPLE)
    # Ramen.app/Contents/MacOS/Ramen, data files go in Ramen.app/Contents.
    set( RAMEN_ICON ${PROJECT_SOURCE_DIR}/graphics/ramen.icns)
    set_source_files_properties( ${RAMEN_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

    add_executable( ramen.bin MACOSX_BUNDLE main.cpp ${RAMEN_ICON})

    set_target_properties( ramen.bin PROPERTIES
                                     OUTPUT_NAME Ramen
                                     MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/cmake/Info.plist.in
                                     MACOSX_BUNDLE_BUNDLE_NAME Ramen
                                     MACOSX_BUNDLE_GUI_IDENTIFIER org.ramen.Ramen
                                     MACOSX_BUNDLE_ICON_FILE ramen.icns
                                     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
                                     MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
                                     INSTALL_RPATH "@executable_path/../Frameworks")

    set( RAMEN_DATA_DIR $<TARGET_BUNDLE_CONTENT_DIR:ramen.bin>)
else()
    # bin/ramen, data files go next to bin.
    add_executable( ramen.bin main.cpp)
    set_target_properties( ramen.bin PROPERTIES OUTPUT_NAME ramen)

    set( RAMEN_DATA_DIR ${PROJECT_BINARY_DIR})
endif()

# nodes, image formats and Qt resources register themselves from static
# initializers that nothing else references, link the whole library.
target_link_libraries( ramen.bin PRIVATE "$<LINK_LIBRARY:WHOLE_ARCHIVE,ramen>")

######################################################
# data files

add_custom_command( TARGET ramen.bin POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/ocio ${RAMEN_DATA_DIR}/ocio
                    VERBATIM)

######################################################
# install

include( GNUInstallDirs)

if( APPLE)
    install( TARGETS ramen.bin BUNDLE DESTINATION .)
    install( DIRECTORY ${PROJECT_SOURCE_DIR}/ocio DESTINATION Ramen.app/Contents)
else()
    install( TARGETS ramen.bin RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    install( DIRECTORY ${PROJECT_SOURCE_DIR}/ocio DESTINATION .)
endif()

# copy Qt and its plugins next to the executable ( windeployqt, macdeployqt, ...).
if( Qt6_VERSION VERSION_GREATER_EQUAL 6.5)
    qt_generate_deploy_app_script( TARGET ramen.bin
                                   OUTPUT_SCRIPT RAMEN_DEPLOY_SCRIPT
                                   NO_UNSUPPORTED_PLATFORM_ERROR)

    install( SCRIPT ${RAMEN_DEPLOY_SCRIPT})
endif()
