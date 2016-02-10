cmake_minimum_required(VERSION 3.3)
set(PROJECT_NAME "sfray")
project(${PROJECT_NAME})

# Compiler Flags
SET(GCC_COVERAGE_COMPILE_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS} -O1")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -std=c++11")

# SFML Include
add_definitions(-DSFML_STATIC)
include_directories("C:/SFML-2.3/include")
link_directories(C:/SFML-2.3/lib)
SET(GCC_COVERAGE_LINK_FLAGS "${GCC_COVERAGE_LINK_FLAGS} -static-libgcc -static-libstdc++")

# Comment Out for Debug
#SET(GCC_COVERAGE_LINK_FLAGS "${GCC_COVERAGE_LINK_FLAGS} -mwindows")

SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

set(SOURCE_FILES main.cpp
        Map.cpp Map.hpp
        Raycaster.cpp Raycaster.hpp
        Camera.cpp Camera.hpp
        Entity.cpp Entity.hpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
        sfml-graphics-s
        sfml-window-s
        sfml-system-s
        glu32
        freetype
        opengl32
        jpeg
        winmm
        gdi32)

# copy resource files
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" VERBATIM)