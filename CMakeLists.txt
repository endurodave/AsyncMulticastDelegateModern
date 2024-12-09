# Example CMake command line to create project build files:
#
# *** Windows ***
# cmake -G "Visual Studio 17 2022" -A Win32 -B build -S .
# cmake -G "Visual Studio 17 2022" -A x64 -B build -S .
# cmake -G "Visual Studio 17 2022" -A x64 -B build -S . -DENABLE_UNIT_TESTS=ON
# cmake -G "Visual Studio 17 2022" -A x64 -B build -S . -DENABLE_ALLOCATOR=ON
#
# *** Linux ***
# cmake -G "Unix Makefiles" -B build -S .
# cmake -G "Unix Makefiles" -B build -S . -DENABLE_UNIT_TESTS=ON
# cmake -G "Unix Makefiles" -B build -S . -DENABLE_ALLOCATOR=ON

# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and language (C or C++)
project(Delegate VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Collect all .cpp and *.h source files in the src directory
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.h")

# Add subdirectories to include path
include_directories( 
    ${CMAKE_SOURCE_DIR}/src/Delegate
    ${CMAKE_SOURCE_DIR}/src/Examples
    ${CMAKE_SOURCE_DIR}/src/Port
)

if (ENABLE_ALLOCATOR)
include_directories( 
    ${CMAKE_SOURCE_DIR}/src/Allocator
)
endif()

# Add an executable target
add_executable(DelegateApp ${SOURCES})

# Define the DELEGATE_UNIT_TEST macro for the DelegateApp target
if (ENABLE_UNIT_TESTS)
    add_compile_definitions(DELEGATE_UNIT_TESTS)
endif()

if (ENABLE_ALLOCATOR)
    add_compile_definitions(USE_ALLOCATOR)
endif()

# Add subdirectories to build
add_subdirectory(src/Delegate)
add_subdirectory(src/Examples)
add_subdirectory(src/Port)

if (ENABLE_ALLOCATOR)
    add_subdirectory(src/Allocator)
endif()

target_link_libraries(DelegateApp PRIVATE 
    DelegateLib
    ExamplesLib
    PortLib
)

if (ENABLE_ALLOCATOR)
    target_link_libraries(DelegateApp PRIVATE 
        AllocatorLib
)
endif()

