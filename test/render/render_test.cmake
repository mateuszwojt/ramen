file( REMOVE ${OUTPUT})

execute_process( COMMAND ${RAMEN} -render -frames 1 1 ${COMP} RESULT_VARIABLE result)

if( NOT result EQUAL 0)
    message( FATAL_ERROR "ramen -render failed: ${result}")
endif()

if( NOT EXISTS ${OUTPUT})
    message( FATAL_ERROR "ramen -render did not write ${OUTPUT}")
endif()
