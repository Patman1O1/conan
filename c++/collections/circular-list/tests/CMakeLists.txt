#-----------------------------------------------------------------------------------------------------------------------
# Google Test
#-----------------------------------------------------------------------------------------------------------------------
include(GoogleTest)
enable_testing()

#-----------------------------------------------------------------------------------------------------------------------
# Target: circular_list_test
# Alias: collections::circular_list_test
# Type: Executable
# Dependencies: GTest::gtest_main, GTest::gmock_main, collections::circular_list_headers
#-----------------------------------------------------------------------------------------------------------------------
# Target definition
add_executable(circular_list_test)

# Target alias
add_executable(collections::circular_list_test ALIAS circular_list_test)

# Target sources
target_sources(circular_list_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/circular_list_test.cpp
)

# Target dependencies
target_link_libraries(circular_list_test
    PRIVATE
        collections::circular_list_headers
        GTest::gtest_main
        GTest::gmock_main
)

# Test discovery
gtest_discover_tests(circular_list_test)
