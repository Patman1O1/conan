#-----------------------------------------------------------------------------------------------------------------------
# Google Test
#-----------------------------------------------------------------------------------------------------------------------
include(GoogleTest)
enable_testing()

#-----------------------------------------------------------------------------------------------------------------------
# Target: list_test
# Alias: collections::list_test
# Type: Executable
# Dependencies: GTest::gtest_main, GTest::gmock_main, collections::list_headers
#-----------------------------------------------------------------------------------------------------------------------
# Target definition
add_executable(list_test)

# Target alias
add_executable(collections::list_test ALIAS list_test)

# Target sources
target_sources(list_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/list_test.cpp
)

# Target dependencies
target_link_libraries(list_test
    PRIVATE
        collections::list_headers
        GTest::gtest_main
        GTest::gmock_main
)

# Test discovery
gtest_discover_tests(list_test)
