# ── Google Test ──────────────────────────────────────────────────────────────
include(GoogleTest)
enable_testing()

# ───────────────────────────────────────────────────────────────────────────── 
# Target: cpp_hive_test
# Alias: cpp_hive::cpp_hive_test
# Type: Executable
# Dependencies: GTest::gtest_main,
#               GTest::gmock_main,
#               cpp_hive::cpp_hive_headers
# ─────────────────────────────────────────────────────────────────────────────
# Target definition
add_executable(cpp_hive_test)

# Target alias
add_executable(cpp_hive::cpp_hive_test
    ALIAS cpp_hive_test
)

# Target sources
target_sources(cpp_hive_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp_hive_test.cpp
)

# Target dependencies
target_link_libraries(cpp_hive_test
    PRIVATE
        cpp_hive::cpp_hive_headers
        GTest::gtest_main
        GTest::gmock_main
)

# Test discovery
gtest_discover_tests(cpp_hive_test)
