# ── Information ──────────────────────────────────────────────────────────────
cmake_minimum_required(VERSION 4.3.0)
project("cpp_deque"
        VERSION 0.1.0
        DESCRIPTION ""
        LANGUAGES CXX
)

# CMake Information
cmake_file_api(
        QUERY
        API_VERSION 1
        CODEMODEL 2.3
)

# ── Settings ─────────────────────────────────────────────────────────────────
if(PROJECT_IS_TOP_LEVEL)
    # C++ Settings
    set(CMAKE_CXX_STANDARD 26)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    # Ensure the current source directory contains the .clang directory
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.clang)

    # Create a symlink to compile_commands.json file
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
            ${CMAKE_CURRENT_SOURCE_DIR}/.clang/compile_commands.json
    )

    # Project Settings
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

    # Project Options
    option(BUILD_TESTS "Build the project's test suite" OFF)

    # Set default build type if not specified
    if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
        set_property(
            CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug"
                                                    "Release"
                                                    "MinSizeRel"
                                                    "RelWithDebInfo"
        )
    endif()
endif()

# ── Dependencies ─────────────────────────────────────────────────────────────
if(BUILD_TESTS)
    find_package(GTest REQUIRED)
endif()

include(GNUInstallDirs)

# ── Installation ─────────────────────────────────────────────────────────────
if(NOT CMAKE_SKIP_INSTALL_RULES)
    # Install headers
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
            TYPE INCLUDE)
endif()

# ─────────────────────────────────────────────────────────────────────────────
# Target: cpp_deque_headers
# Alias: cpp_deque::cpp_deque_headers
# Type: Interface Library
# ───────────────────────────────────────────────────────────────────────────── 
# Target definition
add_library(cpp_deque_headers INTERFACE)

# Target alias
add_library(cpp_deque::cpp_deque_headers 
    ALIAS
        cpp_deque_headers
)

# Target headers
target_include_directories(cpp_deque_headers
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# ─────────────────────────────────────────────────────────────────────────────
# Target: cpp_deque
# Alias: cpp_deque::cpp_deque
# Type: Interface Library
# Dependencies: cpp_deque::cpp_deque_headers
# ─────────────────────────────────────────────────────────────────────────────
# Target definition
add_library(cpp_deque INTERFACE)

# Target alias
add_library(cpp_deque::cpp_deque
    ALIAS
        cpp_deque
)

# Target dependencies
target_link_libraries(cpp_deque
    INTERFACE
        cpp_deque::cpp_deque_headers
)

# Target properties
set_target_properties(cpp_deque PROPERTIES
        OUTPUT_NAME cpp_deque
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
)

if(NOT CMAKE_SKIP_INSTALL_RULES)
    # Install cpp_deque
    install(TARGETS cpp_deque
            RUNTIME COMPONENT cpp_deque
            LIBRARY COMPONENT cpp_deque
            NAMELINK_COMPONENT cpp_deque
            ARCHIVE COMPONENT cpp_deque
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

# ── Subdirectories ───────────────────────────────────────────────────────────
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
