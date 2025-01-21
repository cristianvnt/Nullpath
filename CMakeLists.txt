cmake_minimum_required(VERSION 3.28)

project(Abyssal-Fracture LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug, Release)" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

#Define resource dirs
set(RESOURCES_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/resources")
set(RESOURCES_OUTPUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

#Copy resource dirs
file(COPY ${RESOURCES_SOURCE_DIR} DESTINATION ${RESOURCES_OUTPUT_DIR})

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

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/game/**/*.cpp")

add_executable(main ${SOURCES})

target_link_libraries(main PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

#Build type-specific settings
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(STATUS "Config for Debug mode with MSVC")
        target_compile_options(main PRIVATE /Zi /Od)
        target_link_options(main PRIVATE /DEBUG)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        message(STATUS "Config for Release mode with MSVC")
        target_compile_options(main PRIVATE /O2)
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(STATUS "Config for Debug mode with GCC/Clang")
        target_compile_options(main PRIVATE -g)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        message(STATUS "Config for Release mode with GCC/Clang")
        target_compile_options(main PRIVATE -O3)
    endif()
endif()