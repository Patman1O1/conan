#-----------------------------------------------------------------------------------------------------------------------
# Google Test
#-----------------------------------------------------------------------------------------------------------------------
include(GoogleTest)
enable_testing()

#-----------------------------------------------------------------------------------------------------------------------
# Target: set_test
# Alias: collections::set_test
# Type: Executable
# Dependencies: GTest::gtest_main, GTest::gmock_main, collections::set_headers
#-----------------------------------------------------------------------------------------------------------------------
# Target definition
add_executable(set_test)

# Target alias
add_executable(collections::set_test ALIAS set_test)

# Target sources
target_sources(set_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/set_test.cpp
)

# Target dependencies
target_link_libraries(set_test
    PRIVATE
        collections::set_headers
        GTest::gtest_main
        GTest::gmock_main
)

# Test discovery
gtest_discover_tests(set_test)
