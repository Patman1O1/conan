# ── Google Test ──────────────────────────────────────────────────────────────
include(GoogleTest)
enable_testing()

# ───────────────────────────────────────────────────────────────────────────── 
# Target: cpp_deque_test
# Alias: cpp_deque::cpp_deque_test
# Type: Executable
# Dependencies: GTest::gtest_main,
#               GTest::gmock_main,
#               cpp_deque::cpp_deque_headers
# ─────────────────────────────────────────────────────────────────────────────
# Target definition
add_executable(cpp_deque_test)

# Target alias
add_executable(cpp_deque::cpp_deque_test
    ALIAS cpp_deque_test
)

# Target sources
target_sources(cpp_deque_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp_deque_test.cpp
)

# Target dependencies
target_link_libraries(cpp_deque_test
    PRIVATE
        cpp_deque::cpp_deque_headers
        GTest::gtest_main
        GTest::gmock_main
)

# Test discovery
gtest_discover_tests(cpp_deque_test)
