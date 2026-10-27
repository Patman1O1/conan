# ── Google Test ──────────────────────────────────────────────────────────────
include(GoogleTest)
enable_testing()

# ───────────────────────────────────────────────────────────────────────────── 
# Target: circular_inplace_vector_test
# Alias: collections::circular_inplace_vector_test
# Type: Executable
# Dependencies: GTest::gtest_main,
#               GTest::gmock_main,
#               collections::circular_inplace_vector_headers
# ─────────────────────────────────────────────────────────────────────────────
# Target definition
add_executable(circular_inplace_vector_test)

# Target alias
add_executable(collections::circular_inplace_vector_test
    ALIAS circular_inplace_vector_test
)

# Target sources
target_sources(circular_inplace_vector_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/circular_inplace_vector_test.cpp
)

# Target dependencies
target_link_libraries(circular_inplace_vector_test
    PRIVATE
        collections::circular_inplace_vector_headers
        GTest::gtest_main
        GTest::gmock_main
)

# Test discovery
gtest_discover_tests(circular_inplace_vector_test)
