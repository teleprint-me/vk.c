# Vulkan Compute CMakeLists.txt

cmake_minimum_required(VERSION 3.14)
project("vk" VERSION 0.1.0 DESCRIPTION "ANSI C library for Number Generation")

# Set C standard rules
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Setup submodule, module, and test names
set(MODULES "vk-instance") # Add more modules here as needed

# Set the output directory for built binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Enable Shared Objects
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# Add examples
add_subdirectory(examples)

# Collect all source files into lists
foreach(MODULE IN LISTS MODULES)
    list(APPEND LIB_HEADERS "include/${MODULE}.h")
    list(APPEND LIB_SOURCES "src/${MODULE}.c")
endforeach()

# Add the library using the collected sources
add_library("vk" ${LIB_SOURCES})

set_target_properties(
    "vk" PROPERTIES
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER "${LIB_HEADERS}"
)

target_include_directories("vk" PUBLIC include)
target_link_libraries("vk" PUBLIC m rt vulkan)
