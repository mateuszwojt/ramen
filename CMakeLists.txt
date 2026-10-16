######################################################################
# Copyright (C) 2013 Esteban Tovagliari. All Rights Reserved.        #
# Licensed under the terms of the CDDL License.                      #
# See CDDL_LICENSE.txt for a copy of the license.                    #
######################################################################

cmake_minimum_required( VERSION 3.24)

project( ramen VERSION 0.9.5 LANGUAGES C CXX)

######################################################
# cmake stuff

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

if( PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message( FATAL_ERROR "CMake generation for Ramen is not allowed within the source directory!")
endif()

get_property( RAMEN_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

if( NOT RAMEN_MULTI_CONFIG AND NOT CMAKE_BUILD_TYPE)
    set( CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

######################################################
# version

set( RAMEN_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set( RAMEN_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set( RAMEN_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set( RAMEN_VERSION ${PROJECT_VERSION})

####################################################################
# options

option( WITH_TESTS "Build tests" ON)

####################################################################
# compiler

set( CMAKE_CXX_STANDARD 20)
set( CMAKE_CXX_STANDARD_REQUIRED ON)
set( CMAKE_CXX_EXTENSIONS OFF)

set( CMAKE_AUTOMOC ON)
set( CMAKE_AUTOUIC ON)
set( CMAKE_AUTORCC ON)
set( CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/designer)

# Ramen looks for its data files relative to the executable ( bin/..).
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

####################################################################
# dependencies

find_package( Threads REQUIRED)

find_package( OpenGL REQUIRED)
find_package( GLEW REQUIRED)

# Boost.GIL ( header only) is the only Boost library used.
find_package( Boost CONFIG REQUIRED)
find_package( TBB CONFIG REQUIRED)
find_package( yaml-cpp CONFIG REQUIRED)

find_package( Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)

find_package( Imath CONFIG REQUIRED)
find_package( OpenEXR CONFIG REQUIRED)

find_package( JPEG REQUIRED)
find_package( PNG REQUIRED)
find_package( TIFF REQUIRED)
find_package( ZLIB REQUIRED)
find_package( OpenImageIO CONFIG REQUIRED)

find_package( OpenColorIO CONFIG REQUIRED)

find_package( Agg REQUIRED)

# the SSE2 image processing code runs on top of NEON on arm.
if( CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm64|aarch64|ARM64)$")
    find_path( SSE2NEON_INCLUDE_DIR NAMES sse2neon.h PATH_SUFFIXES sse2neon REQUIRED)
endif()

######################################################
# ramen

add_subdirectory( ramen)

######################################################
# tests

if( WITH_TESTS)
    find_package( GTest CONFIG REQUIRED)

    enable_testing()
    add_subdirectory( test)
endif()

######################################################
# docs

find_package( Doxygen)

if( DOXYGEN_FOUND)
    configure_file( ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target( doc ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
                        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                        COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif()
