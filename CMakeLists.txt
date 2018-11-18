# Configuration

cmake_minimum_required(VERSION 3.1)

project(Fortune)

# Compiler 

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Files

file(GLOB_RECURSE SRCS src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)

# Search directories

include_directories(src/)

# Executable

set(EXECUTABLE_NAME "Fortune")
add_executable(${EXECUTABLE_NAME} ${SRCS} ${HEADERS})

# Libraries

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2.4 REQUIRED network audio graphics window system)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
else()
    message(FATAL_ERROR "SFML not found")
endif()

