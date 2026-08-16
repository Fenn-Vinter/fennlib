cmake_minimum_required(VERSION 3.14)
project(fennlib_project LANGUAGES CXX)

# Force a modern C++ baseline 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Header-only Library Target (Always created)
add_library(fennlib INTERFACE)

# Expose the "include" directory to downstream consumers
target_include_directories(fennlib INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Apply cross-compiler fixes
if(MSVC)
    target_compile_options(fennlib INTERFACE /Zc:__cplusplus)
else()
    target_compile_options(fennlib INTERFACE -Wall -Wextra)
endif()

# 2. Conditional Compilation Block
# Only compile the test executable if this is the top-level project
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(STATUS "[fennlib] Building standalone with test executable.")
    
    add_executable(test_runner test/main.cpp)
    target_link_libraries(test_runner PRIVATE fennlib)
else()
    message(STATUS "[fennlib] Included as a subdirectory dependency. Skipping tests.")
endif()
