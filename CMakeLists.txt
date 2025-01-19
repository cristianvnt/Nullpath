cmake_minimum_required(VERSION 3.28)

project(Abyssal-Fracture LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
