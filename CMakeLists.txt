cmake_minimum_required(VERSION 3.28)

project(Nullpath LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug, Release)" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/**/*.cpp")

file(COPY ${CMAKE_SOURCE_DIR}/src/Config DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${CMAKE_SOURCE_DIR}/src/Resources DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(Nullpath ${SOURCES})
target_include_directories(Nullpath PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(Nullpath PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

# Build type-specific settings
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(STATUS "Debug mode with MSVC")
        target_compile_options(Nullpath PRIVATE /Zi /Od)
        target_link_options(Nullpath PRIVATE /DEBUG /INCREMENTAL)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        message(STATUS "Release mode with MSVC")
        target_compile_options(Nullpath PRIVATE /O2)
    endif()
endif()
