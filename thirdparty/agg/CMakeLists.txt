######################################################################
# Anti-Grain Geometry 2.4 ( see README.md)
######################################################################

add_library( agg STATIC src/agg_arc.cpp
                        src/agg_bezier_arc.cpp
                        src/agg_curves.cpp
                        src/agg_trans_affine.cpp
                        src/agg_vcgen_stroke.cpp
                        )

target_include_directories( agg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# the sources include the headers without the agg2 prefix.
target_include_directories( agg PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/agg2)

set_target_properties( agg PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library( Agg::Agg ALIAS agg)
