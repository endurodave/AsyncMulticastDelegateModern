# Example CMake command line to create project build files:
#
# *** Windows ***
# cmake -G "Visual Studio 17 2022" -A x64 .
# cmake -G "Visual Studio 17 2022" -A Win32 .
# cmake -G "Visual Studio 17 2022" -A x64 . -DENABLE_UNIT_TESTS=ON
#
# *** Linux ***
# cmake -G "Unix Makefiles" .
# cmake -G "Unix Makefiles" . -DENABLE_UNIT_TESTS=ON

# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and language (C or C++)
project(Delegate VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Collect all .cpp source files in the current directory
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/*.cpp")

# Add subdirectories to include path
include_directories( 
    ${CMAKE_SOURCE_DIR}/Delegate
    ${CMAKE_SOURCE_DIR}/Examples
    ${CMAKE_SOURCE_DIR}/Port
)

# Add an executable target
add_executable(DelegateApp ${SOURCES})

# Define the DELEGATE_UNIT_TEST macro for the DelegateApp target
if (ENABLE_UNIT_TESTS)
    target_compile_definitions(DelegateApp PRIVATE DELEGATE_UNIT_TESTS)
endif()

# Add /bigobj flag for MSVC and ENABLE_UNIT_TESTS because unit tests are large
if (MSVC AND ENABLE_UNIT_TESTS)
    target_compile_options(DelegateApp PRIVATE /bigobj)
endif()

# Add subdirectories to build
add_subdirectory(Delegate)
add_subdirectory(Examples)
add_subdirectory(Port)

target_link_libraries(DelegateApp PRIVATE 
    DelegateLib
    ExamplesLib
    PortLib
)

