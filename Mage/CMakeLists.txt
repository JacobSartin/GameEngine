cmake_minimum_required( VERSION 4.2 )

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

file(GLOB_RECURSE PUBLIC_HEADERS CONFIGURE_DEPENDS "src/*.h")

set(MODULES_PATH "src")
# Glob all modules then pass them to target_sources so CMake can scan and compile them correctly
file(GLOB_RECURSE MODULE_FILES CONFIGURE_DEPENDS
    "${MODULES_PATH}/*.cppm"
)
# Keep module sources explicit so CMake scans and compiles them correctly.
foreach(module_file ${MODULE_FILES})
    set_source_files_properties(${module_file} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
endforeach()

add_library( Mage SHARED )

set_property(TARGET Mage PROPERTY LINKER_LANGUAGE CXX )
set_property(TARGET Mage PROPERTY CXX_STANDARD 20)
set_property(TARGET Mage PROPERTY CXX_STANDARD_REQUIRED ON)
# set_property(TARGET Mage PROPERTY CXX_EXTENSIONS OFF)

target_sources( Mage
    PRIVATE
        ${PROJECT_SOURCES}
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "src"
        FILES
            ${PUBLIC_HEADERS}
    PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${MODULES_PATH}
        FILES
            ${MODULE_FILES}
)

# Find packages via vcpkg
find_package( OpenGL REQUIRED )
find_package( SDL2 CONFIG REQUIRED )
find_package( GLEW CONFIG REQUIRED )
find_package( Freetype REQUIRED )
find_package( glm CONFIG REQUIRED )
find_package( spdlog CONFIG REQUIRED )
find_package( Stb REQUIRED )

target_link_libraries( Mage PUBLIC
    SDL2::SDL2
    glm::glm
)
target_link_libraries( Mage PRIVATE
    ${OPENGL_LIBRARIES}
    GLEW::GLEW
    Freetype::Freetype
    spdlog::spdlog
)

# Stb is header-only — no link target, just expose the include path
target_include_directories( Mage PRIVATE ${STB_INCLUDE_DIRS} )

add_compile_definitions( MAGE_LIBRARY_BUILD )

set( SDL_SHARED OFF )
set( SDL_STATIC ON )
set( GLEW_USE_STATIC_LIBS ON )
set( BUILD_SHARED_LIBS OFF ) # freetype