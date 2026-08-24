cmake_minimum_required(VERSION 3.14)
project(fennlib_project LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(fennlib INTERFACE)
add_library(fennlib::fennlib ALIAS fennlib)

target_include_directories(fennlib INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if(MSVC)
    target_compile_options(fennlib INTERFACE /Zc:__cplusplus)
else()
    target_compile_options(fennlib INTERFACE -Wall -Wextra)
endif()

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(STATUS "[fennlib] Building standalone with test executable.")
    
    add_executable(test_runner test/main.cpp)
    target_link_libraries(test_runner PRIVATE fennlib)
else()
    message(STATUS "[fennlib] Included as a subdirectory dependency. Skipping tests.")
endif()