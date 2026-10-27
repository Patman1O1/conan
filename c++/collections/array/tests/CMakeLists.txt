# ── Google Test ──────────────────────────────────────────────────────────────
include(GoogleTest)
enable_testing()

# ─────────────────────────────────────────────────────────────────────────────
# Target: array_tests
# Alias: collections::array_tests
# Type: Executable
# Dependencies: GTest::gtest_main,
#               GTest::gmock_main,
#               collections::array_headers
# ─────────────────────────────────────────────────────────────────────────────
# Target definition
add_executable(array_tests)

# Target alias
add_executable(collections::array_tests
        ALIAS array_tests
)

# Target sources
target_sources(array_tests
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/array_tests.cpp
)

# Target dependencies
target_link_libraries(array_tests
        PRIVATE
            collections::array_headers
            GTest::gtest_main
            GTest::gmock_main
)

# Test discovery
gtest_discover_tests(array_tests)