cmake_minimum_required( VERSION 4.2 )

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# project must come before stuff that depends on project settings
project( "GameEngine" VERSION 1.0.0 LANGUAGES CXX)


# Maximum warning output from MSVC/gcc
if( MSVC )
    add_definitions( -D_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS )
    string(REGEX REPLACE "/W[1|2|3|4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    string(REGEX REPLACE "/W[1|2|3|4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    # Disable warning C5272 globally for MSVC builds
    add_compile_options(/wd5272)
else()
    add_compile_options( -Wall -Wextra -Wpedantic )
endif()


add_subdirectory( "Mage" )
add_subdirectory( "Game" )
