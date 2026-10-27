#-----------------------------------------------------------------------------------------------------------------------
# Google Test
#-----------------------------------------------------------------------------------------------------------------------
include(GoogleTest)
enable_testing()

#-----------------------------------------------------------------------------------------------------------------------
# Target: forward_list_test
# Alias: collections::forward_list_test
# Type: Executable
# Dependencies: GTest::gtest_main, GTest::gmock_main, collections::forward_list_headers
#-----------------------------------------------------------------------------------------------------------------------
# Target definition
add_executable(forward_list_test)

# Target alias
add_executable(collections::forward_list_test ALIAS forward_list_test)

# Target sources
target_sources(forward_list_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/forward_list_test.cpp
)

# Target dependencies
target_link_libraries(forward_list_test
    PRIVATE
        collections::forward_list_headers
        GTest::gtest_main
        GTest::gmock_main
)

# Test discovery
gtest_discover_tests(forward_list_test)
