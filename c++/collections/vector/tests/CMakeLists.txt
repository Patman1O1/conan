#-----------------------------------------------------------------------------------------------------------------------
# Google Test
#-----------------------------------------------------------------------------------------------------------------------
include(GoogleTest)
enable_testing()

#-----------------------------------------------------------------------------------------------------------------------
# Target: vector_test
# Alias: collections::vector_test
# Type: Executable
# Dependencies: GTest::gtest_main, GTest::gmock_main, collections::vector_headers
#-----------------------------------------------------------------------------------------------------------------------
# Target definition
add_executable(vector_test)

# Target alias
add_executable(collections::vector_test ALIAS vector_test)

# Target sources
target_sources(vector_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/vector_test.cpp
)

# Target dependencies
target_link_libraries(vector_test
    PRIVATE
        collections::vector_headers
        GTest::gtest_main
        GTest::gmock_main
)

# Test discovery
gtest_discover_tests(vector_test)
