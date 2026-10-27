# ─────────────────────────────────────────────────────────────────────────────
# Target: test_package
# Type: Executable
# Dependencies: cpp_hive::cpp_hive
# ─────────────────────────────────────────────────────────────────────────────
# Target definition
add_executable(test_package)

# Target sources
target_sources(test_package
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# Target dependencies
target_link_libraries(test_package
    PRIVATE
        cpp_hive::cpp_hive
)
