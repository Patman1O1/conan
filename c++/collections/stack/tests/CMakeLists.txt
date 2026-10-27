#-----------------------------------------------------------------------------------------------------------------------
# Google Test
#-----------------------------------------------------------------------------------------------------------------------
include(GoogleTest)
enable_testing()

#-----------------------------------------------------------------------------------------------------------------------
# Target: stack_test
# Alias: collections::stack_test
# Type: Executable
# Dependencies: GTest::gtest_main, GTest::gmock_main, collections::stack_headers
#-----------------------------------------------------------------------------------------------------------------------
# Target definition
add_executable(stack_test)

# Target alias
add_executable(collections::stack_test ALIAS stack_test)

# Target sources
target_sources(stack_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/stack_test.cpp
)

# Target dependencies
target_link_libraries(stack_test
    PRIVATE
        collections::stack_headers
        GTest::gtest_main
        GTest::gmock_main
)

# Test discovery
gtest_discover_tests(stack_test)
