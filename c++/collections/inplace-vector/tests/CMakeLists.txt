# ── Google Test ──────────────────────────────────────────────────────────────
include(GoogleTest)
enable_testing()

# ───────────────────────────────────────────────────────────────────────────── 
# Target: inplace_vector_test
# Alias: collections::inplace_vector_test
# Type: Executable
# Dependencies: GTest::gtest_main,
#               GTest::gmock_main,
#               collections::inplace_vector_headers
# ─────────────────────────────────────────────────────────────────────────────
# Target definition
add_executable(inplace_vector_test)

# Target alias
add_executable(collections::inplace_vector_test
    ALIAS inplace_vector_test
)

# Target sources
target_sources(inplace_vector_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inplace_vector_test.cpp
)

# Target dependencies
target_link_libraries(inplace_vector_test
    PRIVATE
        collections::inplace_vector_headers
        GTest::gtest_main
        GTest::gmock_main
)

# Test discovery
gtest_discover_tests(inplace_vector_test)
