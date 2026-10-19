file( REMOVE_RECURSE ${OUTPUT_DIR})
file( MAKE_DIRECTORY ${OUTPUT_DIR})

execute_process( COMMAND ${RAMEN} -render -frames ${FIRST} ${LAST} ${COMP}
                 RESULT_VARIABLE result
                 OUTPUT_QUIET)

if( NOT result EQUAL 0)
    message( FATAL_ERROR "ramen -render failed: ${result}")
endif()

file( GLOB references RELATIVE ${REFERENCE_DIR} ${REFERENCE_DIR}/*.exr)

if( NOT references)
    message( FATAL_ERROR "no reference images in ${REFERENCE_DIR}")
endif()

set( failed)

foreach( image ${references})
    if( NOT EXISTS ${OUTPUT_DIR}/${image})
        list( APPEND failed ${image})
        message( "${image} was not rendered")
        continue()
    endif()

    execute_process( COMMAND ${EXR_COMPARE} ${REFERENCE_DIR}/${image} ${OUTPUT_DIR}/${image} ${TOLERANCE}
                     RESULT_VARIABLE result)

    if( NOT result EQUAL 0)
        list( APPEND failed ${image})
    endif()
endforeach()

if( failed)
    message( FATAL_ERROR "images different from the reference: ${failed}")
endif()
