#-----------------------------------------------------------------------------------------------------------------------
# Google Test
#-----------------------------------------------------------------------------------------------------------------------
include(GoogleTest)
enable_testing()

#-----------------------------------------------------------------------------------------------------------------------
# Target: map_test
# Alias: collections::map_test
# Type: Executable
# Dependencies: GTest::gtest_main, GTest::gmock_main, collections::map_headers
#-----------------------------------------------------------------------------------------------------------------------
# Target definition
add_executable(map_test)

# Target alias
add_executable(collections::map_test ALIAS map_test)

# Target sources
target_sources(map_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/map_test.cpp
)

# Target dependencies
target_link_libraries(map_test
    PRIVATE
        collections::map_headers
        GTest::gtest_main
        GTest::gmock_main
)

# Test discovery
gtest_discover_tests(map_test)
