# Vulkan Compute CMakeLists.txt

cmake_minimum_required(VERSION 3.14)
project("vk" VERSION 0.1.0 DESCRIPTION "ANSI C library for Number Generation")

# Set C standard rules
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Setup submodule, module, and test names
set(MODULES "vk-instance" "vk-device") # Add more modules here as needed

# Set the output directory for built binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Enable Shared Objects
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

find_package(Doxygen)

if(DOXYGEN_FOUND)
    message(STATUS "Doxygen found: ${DOXYGEN_EXECUTABLE}")
    
    # Set input and output directories
    set(DOXYGEN_INPUT_DIR "${CMAKE_SOURCE_DIR}/include")
    set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/docs")
    
    # Configure Doxygen
    set(DOXYGEN_GENERATE_LATEX OFF) # Disable LaTeX output if not needed
    set(DOXYGEN_GENERATE_MAN OFF)   # Disable man pages
    set(DOXYGEN_OUTPUT_DIRECTORY "${DOXYGEN_OUTPUT_DIR}")
    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_EXTRACT_PRIVATE YES)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_XML NO)
    
    # You can customize more Doxygen settings as needed
    # Refer to https://www.doxygen.nl/manual/config.html for options
    
    # Generate the Doxyfile content
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.ini
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY
    )
    
    # Add a custom target to generate documentation
    add_custom_target(doc_doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
    
    # Optionally, make 'docs' a dependency of 'all'
    # Uncomment the following line if you want documentation to be built with the default build
    # add_dependencies(all doc_doxygen)
    
    message(STATUS "Doxygen documentation target 'doc_doxygen' added.")
else()
    message(WARNING "Doxygen not found. Documentation will not be generated.")
endif()

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
