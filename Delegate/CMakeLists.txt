# Collect all .cpp files in this subdirectory
file(GLOB SUBDIR_SOURCES "*.cpp")

# Collect all .h files in this subdirectory
file(GLOB SUBDIR_HEADERS "*.h")

# Create a library target 
add_library(DelegateLib STATIC ${SUBDIR_SOURCES} ${SUBDIR_HEADERS})

# Include directories for the library
target_include_directories(DelegateLib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Add /bigobj flag for MSVC and ENABLE_UNIT_TESTS because unit tests are large
if (MSVC AND ENABLE_UNIT_TESTS)
    target_compile_options(DelegateLib PRIVATE /bigobj)
endif()