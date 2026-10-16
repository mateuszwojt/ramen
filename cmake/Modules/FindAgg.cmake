# Find Antigrain headers and libraries.
#
#  Agg::Agg  - imported target.
#  Agg_FOUND - True if agg found.

find_path( AGG_INCLUDE_DIR NAMES agg2/agg_bspline.h)
find_library( AGG_LIBRARY NAMES agg)

include( FindPackageHandleStandardArgs)
find_package_handle_standard_args( Agg DEFAULT_MSG AGG_LIBRARY AGG_INCLUDE_DIR)

if( Agg_FOUND AND NOT TARGET Agg::Agg)
    add_library( Agg::Agg UNKNOWN IMPORTED)
    set_target_properties( Agg::Agg PROPERTIES
                            IMPORTED_LOCATION "${AGG_LIBRARY}"
                            INTERFACE_INCLUDE_DIRECTORIES "${AGG_INCLUDE_DIR}")
endif()

mark_as_advanced( AGG_INCLUDE_DIR AGG_LIBRARY)
