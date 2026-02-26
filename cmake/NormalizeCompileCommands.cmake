if( NOT DEFINED INPUT_FILE )
    message( FATAL_ERROR "INPUT_FILE was not provided" )
endif()

if( NOT EXISTS "${INPUT_FILE}" )
    message( STATUS "Skipping compile_commands normalization; file does not exist yet: ${INPUT_FILE}" )
    return()
endif()

file( READ "${INPUT_FILE}" compile_commands_content )

set( normalized_content "${compile_commands_content}" )
string( REPLACE "C:/" "c:/" normalized_content "${normalized_content}" )
string( REPLACE "C:\\\\" "c:\\\\" normalized_content "${normalized_content}" )

if( NOT normalized_content STREQUAL compile_commands_content )
    file( WRITE "${INPUT_FILE}" "${normalized_content}" )
    message( STATUS "Normalized drive-letter casing in ${INPUT_FILE}" )
else()
    message( STATUS "No compile_commands normalization changes needed" )
endif()