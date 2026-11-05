# ── Google Test ──────────────────────────────────────────────────────────────
include(GoogleTest)
enable_testing()

# ─────────────────────────────────────────────────────────────────────────────
# Target: btree_tests
# Alias: collections::btree_tests
# Type: Executable
# Dependencies: GTest::gtest_main,
#               GTest::gmock_main,
#               collections::btree_headers
# ─────────────────────────────────────────────────────────────────────────────
# Target definition
add_executable(btree_tests)

# Target alias
add_executable(collections::btree_tests
        ALIAS btree_tests
)

# Target sources
target_sources(btree_tests
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/btree_tests.cpp
)

# Target dependencies
target_link_libraries(btree_tests
        PRIVATE
            collections::btree_headers
            GTest::gtest_main
            GTest::gmock_main
)

# Test discovery
gtest_discover_tests(btree_tests)