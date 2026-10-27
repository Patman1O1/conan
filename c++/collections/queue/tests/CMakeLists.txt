#-----------------------------------------------------------------------------------------------------------------------
# Google Test
#-----------------------------------------------------------------------------------------------------------------------
include(GoogleTest)
enable_testing()

#-----------------------------------------------------------------------------------------------------------------------
# Target: queue_test
# Alias: collections::queue_test
# Type: Executable
# Dependencies: GTest::gtest_main, GTest::gmock_main, collections::queue_headers
#-----------------------------------------------------------------------------------------------------------------------
# Target definition
add_executable(queue_test)

# Target alias
add_executable(collections::queue_test ALIAS queue_test)

# Target sources
target_sources(queue_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/queue_test.cpp
)

# Target dependencies
target_link_libraries(queue_test
    PRIVATE
        collections::queue_headers
        GTest::gtest_main
        GTest::gmock_main
)

# Test discovery
gtest_discover_tests(queue_test)
