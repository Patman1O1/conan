// ISO C Includes

// ISO C++ Includes
#include <algorithm>
#include <functional>
#include <random>
#include <string>
#include <type_traits>
#include <utility>

// Google Test Includes
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// Local Includes
#include <collections/array.hpp>

namespace collections::array_testing {
    namespace {
        // ── Concepts ────────────────────────────────────────────────────────
        template<typename T, typename... Args>
        concept brace_initializable = requires { T{std::declval<Args>()...}; };

        template<typename... Args>
        concept deducible = requires { array{std::declval<Args>()...}; };

        // ── Test Fixtures ───────────────────────────────────────────────────
        class operator_tests : public testing::Test {};

        class method_tests : public testing::Test {};

        class trait_tests : public testing::Test {};
    } // namespace

    // ── Operator Tests ──────────────────────────────────────────────────────
    TEST_F(operator_tests, equals_operator__self_compare) {
        // Trivial Single Element
        constexpr array trivial_single = {1};
        static_assert(trivial_single == trivial_single);
        EXPECT_TRUE(trivial_single == trivial_single);
        
        // Trivial Multiple Elements
        constexpr array trivial_multiple = {1, 2, 3};
        static_assert(trivial_multiple == trivial_multiple);
        EXPECT_TRUE(trivial_multiple == trivial_multiple);

        // Nontrivial Single Element
        const array<std::string, 1> nontrivial_single = {"This is a test"};
        EXPECT_TRUE(nontrivial_single == nontrivial_single);

        // Nontrivial Multiple Elements
        const array<std::string, 3> nontrivial_multiple = {
            "This", "is a", "test"
        };
        EXPECT_TRUE(nontrivial_multiple == nontrivial_multiple); 
    }

    TEST_F(operator_tests, equals_operator__true_case) {
        // Trivial Single Element
        constexpr array trivial_single_lhs = {1};
        constexpr array trivial_single_rhs = {1};
        static_assert(trivial_single_lhs == trivial_single_rhs);
        EXPECT_TRUE(trivial_single_lhs == trivial_single_rhs);
        
        // Trivial Multiple Elements
        constexpr array trivial_multiple_lhs = {1, 2, 3};
        constexpr array trivial_multiple_rhs = {1, 2, 3};
        static_assert(trivial_multiple_lhs == trivial_multiple_rhs);
        EXPECT_TRUE(trivial_multiple_lhs == trivial_multiple_rhs);

        // Nontrivial Single Element
        const array<std::string, 1> nontrivial_single_lhs = {"This is a test"};
        const array<std::string, 1> nontrivial_single_rhs = {"This is a test"};
        EXPECT_TRUE(nontrivial_single_lhs == nontrivial_single_rhs);

        // Nontrivial Multiple Elements
        const array<std::string, 3> nontrivial_multiple_lhs = {
            "This", "is a", "test"
        };
        const array<std::string, 3> nontrivial_multiple_rhs = {
            "This", "is a", "test"
        };
        EXPECT_TRUE(nontrivial_multiple_lhs == nontrivial_multiple_rhs); 
    }

    TEST_F(operator_tests, equals_operator__false_case) {
        // Trivial Single Element
        constexpr array trivial_single_lhs = {1};
        constexpr array trivial_single_rhs = {2};
        static_assert(!(trivial_single_lhs == trivial_single_rhs));
        EXPECT_FALSE(trivial_single_lhs == trivial_single_rhs);
        
        // Trivial Multiple Elements
        constexpr array trivial_multiple_lhs = {1, 2, 3};
        constexpr array trivial_multiple_rhs = {4, 5, 6};
        static_assert(!(trivial_multiple_lhs == trivial_multiple_rhs));
        EXPECT_FALSE(trivial_multiple_lhs == trivial_multiple_rhs);

        // Nontrivial Single Element
        const array<std::string, 1> nontrivial_single_lhs = {"This is a test"};
        const array<std::string, 1> nontrivial_single_rhs = {
            "This is a different test"
        };
        EXPECT_FALSE(nontrivial_single_lhs == nontrivial_single_rhs);

        // Nontrivial Multiple Elements
        const array<std::string, 3> nontrivial_multiple_lhs = {
            "This", "is a", "test"
        };
        const array<std::string, 3> nontrivial_multiple_rhs = {
            "This", "is a different", "test"
        };
        EXPECT_FALSE(nontrivial_multiple_lhs == nontrivial_multiple_rhs); 
    }

    TEST_F(operator_tests, equality_operator__not_equals_case) {
        // Trivial Single Element
        constexpr array trivial_single_lhs = {1};
        constexpr array trivial_single_rhs = {2};
        static_assert(trivial_single_lhs != trivial_single_rhs);
        static_assert(!(trivial_single_lhs == trivial_single_rhs));
        EXPECT_TRUE(trivial_single_lhs != trivial_single_rhs);
        EXPECT_TRUE(!(trivial_single_lhs == trivial_single_rhs));
 
        // Trivial Multiple Elements
        constexpr array trivial_multiple_lhs = {1, 2, 3};
        constexpr array trivial_multiple_first_rhs = {0, 2, 3};
        constexpr array trivial_multiple_middle_rhs = {1, 0, 3};
        constexpr array trivial_multiple_last_rhs = {1, 2, 0};
 
        static_assert(trivial_multiple_lhs != trivial_multiple_first_rhs);
        static_assert(trivial_multiple_lhs != trivial_multiple_middle_rhs);
        static_assert(trivial_multiple_lhs != trivial_multiple_last_rhs);
        static_assert(!(trivial_multiple_lhs == trivial_multiple_first_rhs));
        static_assert(!(trivial_multiple_lhs == trivial_multiple_middle_rhs));
        static_assert(!(trivial_multiple_lhs == trivial_multiple_last_rhs));
 
        EXPECT_TRUE(trivial_multiple_lhs != trivial_multiple_first_rhs);
        EXPECT_TRUE(trivial_multiple_lhs != trivial_multiple_middle_rhs);
        EXPECT_TRUE(trivial_multiple_lhs != trivial_multiple_last_rhs);
        EXPECT_TRUE(!(trivial_multiple_lhs == trivial_multiple_first_rhs));
        EXPECT_TRUE(!(trivial_multiple_lhs == trivial_multiple_middle_rhs));
        EXPECT_TRUE(!(trivial_multiple_lhs == trivial_multiple_last_rhs));
 
        // Nontrivial Multiple Elements
        const array<std::string, 3> nontrivial_multiple_lhs = {
            "This", "is a", "test"
        };
        const array<std::string, 3> nontrivial_multiple_rhs = {
            "This", "is a", "text"
        };
 
        EXPECT_TRUE(nontrivial_multiple_lhs != nontrivial_multiple_rhs);
        EXPECT_TRUE(!(nontrivial_multiple_lhs == nontrivial_multiple_rhs));
    }
 
    TEST_F(operator_tests, spaceship_operator__less_case) {
        // Trivial Single Element
        constexpr array trivial_single_lhs = {1};
        constexpr array trivial_single_rhs = {2};
        static_assert(trivial_single_lhs <=> trivial_single_rhs < 0);
        EXPECT_TRUE(trivial_single_lhs <=> trivial_single_rhs < 0);
 
        // Trivial Multiple Elements
        constexpr array trivial_multiple_lhs = {1, 2, 3};
        constexpr array trivial_multiple_rhs = {1, 2, 4};
 
        static_assert(trivial_multiple_lhs <=> trivial_multiple_rhs < 0);
        static_assert(trivial_multiple_lhs < trivial_multiple_rhs);
        static_assert(!(trivial_multiple_lhs > trivial_multiple_rhs));
        static_assert(trivial_multiple_lhs <= trivial_multiple_rhs);
        static_assert(!(trivial_multiple_lhs >= trivial_multiple_rhs));
 
        EXPECT_TRUE(trivial_multiple_lhs <=> trivial_multiple_rhs < 0);
        EXPECT_TRUE(trivial_multiple_lhs < trivial_multiple_rhs);
        EXPECT_TRUE(!(trivial_multiple_lhs > trivial_multiple_rhs));
        EXPECT_TRUE(trivial_multiple_lhs <= trivial_multiple_rhs);
        EXPECT_TRUE(!(trivial_multiple_lhs >= trivial_multiple_rhs));
 
        // Trivial Multiple Elements
        constexpr array trivial_lexicographic_lhs = {1, 9, 9};
        constexpr array trivial_lexicographic_rhs = {2, 0, 0};
 
        static_assert(
                trivial_lexicographic_lhs <=> trivial_lexicographic_rhs < 0
        );
        static_assert(trivial_lexicographic_lhs < trivial_lexicographic_rhs);
 
        EXPECT_TRUE(
            trivial_lexicographic_lhs <=> trivial_lexicographic_rhs < 0
        );
        EXPECT_TRUE(trivial_lexicographic_lhs < trivial_lexicographic_rhs);
 
        // Nontrivial Multiple Elements
        const array<std::string, 3> nontrivial_multiple_lhs = {
            "This", "is a", "test"
        };
        const array<std::string, 3> nontrivial_multiple_rhs = {
            "This", "is a", "text"
        };
 
        EXPECT_TRUE(nontrivial_multiple_lhs <=> nontrivial_multiple_rhs < 0);
        EXPECT_TRUE(nontrivial_multiple_lhs < nontrivial_multiple_rhs);
        EXPECT_TRUE(!(nontrivial_multiple_lhs > nontrivial_multiple_rhs));
        EXPECT_TRUE(nontrivial_multiple_lhs <= nontrivial_multiple_rhs);
        EXPECT_TRUE(!(nontrivial_multiple_lhs >= nontrivial_multiple_rhs));
    }
 
    TEST_F(operator_tests, spaceship_operator__greater_case) {
        // Trivial Single Element
        constexpr array trivial_single_lhs = {2};
        constexpr array trivial_single_rhs = {1};
        static_assert(trivial_single_lhs <=> trivial_single_rhs > 0);
        EXPECT_TRUE(trivial_single_lhs <=> trivial_single_rhs > 0);
 
        // Trivial Multiple Elements
        constexpr array trivial_multiple_lhs = {1, 2, 4};
        constexpr array trivial_multiple_rhs = {1, 2, 3};
 
        static_assert(trivial_multiple_lhs <=> trivial_multiple_rhs > 0);
        static_assert(!(trivial_multiple_lhs < trivial_multiple_rhs));
        static_assert(trivial_multiple_lhs > trivial_multiple_rhs);
        static_assert(!(trivial_multiple_lhs <= trivial_multiple_rhs));
        static_assert(trivial_multiple_lhs >= trivial_multiple_rhs);
 
        EXPECT_TRUE(trivial_multiple_lhs <=> trivial_multiple_rhs > 0);
        EXPECT_TRUE(!(trivial_multiple_lhs < trivial_multiple_rhs));
        EXPECT_TRUE(trivial_multiple_lhs > trivial_multiple_rhs);
        EXPECT_TRUE(!(trivial_multiple_lhs <= trivial_multiple_rhs));
        EXPECT_TRUE(trivial_multiple_lhs >= trivial_multiple_rhs);
 
        // Trivial Multiple Elements (first difference decides)
        constexpr array trivial_lexicographic_lhs = {2, 0, 0};
        constexpr array trivial_lexicographic_rhs = {1, 9, 9};
 
        static_assert(
            trivial_lexicographic_lhs <=> trivial_lexicographic_rhs > 0
        );
        static_assert(trivial_lexicographic_lhs > trivial_lexicographic_rhs);
 
        EXPECT_TRUE(
            trivial_lexicographic_lhs <=> trivial_lexicographic_rhs > 0
        );
        EXPECT_TRUE(trivial_lexicographic_lhs > trivial_lexicographic_rhs);
 
        // Nontrivial Multiple Elements
        const array<std::string, 3> nontrivial_multiple_lhs = {
            "This", "is a", "text"
        };
        const array<std::string, 3> nontrivial_multiple_rhs = {
            "This", "is a", "test"
        };
 
        EXPECT_TRUE(nontrivial_multiple_lhs <=> nontrivial_multiple_rhs > 0);
        EXPECT_TRUE(!(nontrivial_multiple_lhs < nontrivial_multiple_rhs));
        EXPECT_TRUE(nontrivial_multiple_lhs > nontrivial_multiple_rhs);
        EXPECT_TRUE(!(nontrivial_multiple_lhs <= nontrivial_multiple_rhs));
        EXPECT_TRUE(nontrivial_multiple_lhs >= nontrivial_multiple_rhs);
    }

    TEST_F(method_tests, subscript_operator__compile_time) {
        constexpr array arr = {1, 2, 3};

        static_assert(arr[0] == 1);
        static_assert(arr[1] == 2);
        static_assert(arr[2] == 3);
    }
    
    // ── Method Tests ────────────────────────────────────────────────────────
    // ── Trait Tests ─────────────────────────────────────────────────────────
    TEST_F(trait_tests, is_aggregate) {
        static_assert(std::is_aggregate_v<array<int, 3>>);
        static_assert(std::is_aggregate_v<array<double, 3>>);
        static_assert(std::is_aggregate_v<array<std::string, 3>>);

        SUCCEED();
    }

    TEST_F(trait_tests, is_brace_initializable) {
        static_assert(
            brace_initializable<array<int, 3>, int, int, int>
        );

        static_assert(
            brace_initializable<array<int, 3>, int, int>
        );

        static_assert(brace_initializable<array<int, 3>, int>);

        static_assert(brace_initializable<array<int, 3>>);

        static_assert(
            !brace_initializable<array<int, 3>, int, int, int, int>
        );

        static_assert(
            !brace_initializable<array<int, 2>, int, std::string>
        );

        SUCCEED();
    }

    TEST_F(trait_tests, no_narrowing_conversions) {
        static_assert(!brace_initializable<array<int, 3>, double>);
        static_assert(!brace_initializable<array<int, 3>, int, double>);
        static_assert(!brace_initializable<array<int, 3>, int, int, double>);

        static_assert(!brace_initializable<array<int, 3>, std::int64_t>);
        static_assert(!brace_initializable<array<int, 3>, int, std::int64_t>);
        static_assert(
            !brace_initializable<array<int, 3>, int, int, std::int64_t>
        );

        SUCCEED();
    }

    TEST_F(trait_tests, is_default_constructible) {
        static_assert(std::is_default_constructible_v<array<int, 3>>);
        static_assert(std::is_default_constructible_v<array<double, 3>>);
        static_assert(std::is_default_constructible_v<array<std::string, 3>>);

        SUCCEED();
    }

    TEST_F(trait_tests, is_trivially_default_constructible) {
        static_assert(
            std::is_trivially_default_constructible_v<array<int, 3>>
        );

        static_assert(
            std::is_trivially_default_constructible_v<array<double, 3>>
        );

        static_assert(
            !std::is_trivially_default_constructible_v<array<std::string, 3>>
        );

        SUCCEED();
    }

    TEST_F(trait_tests, is_copy_constructible) {
        static_assert(std::is_copy_constructible_v<array<int, 3>>);
        static_assert(std::is_copy_constructible_v<array<double, 3>>);
        static_assert(std::is_copy_constructible_v<array<std::string, 3>>);
        static_assert(
            !std::is_copy_constructible_v<array<std::unique_ptr<int>, 3>>
        );
        SUCCEED();
    }

    TEST_F(trait_tests, is_trivially_copy_constructible) {
        static_assert(std::is_trivially_copy_constructible_v<array<int, 3>>);
        static_assert(
            std::is_trivially_copy_constructible_v<array<double, 3>>
        );
        static_assert(
            !std::is_trivially_copy_constructible_v<array<std::string, 3>>
        );
        static_assert(
            !std::is_trivially_copy_constructible_v<
                array<std::unique_ptr<int>, 3>
            >
        );

        SUCCEED();
    }

    TEST_F(trait_tests, is_move_constructible) {
        static_assert(std::is_move_constructible_v<array<int, 3>>);
        static_assert(std::is_move_constructible_v<array<double, 3>>);
        static_assert(std::is_move_constructible_v<array<std::string, 3>>);
        static_assert(
            std::is_move_constructible_v<array<std::unique_ptr<int>, 3>>
        );
    }

    TEST_F(trait_tests, is_trivially_move_constructible) {
        static_assert(std::is_trivially_move_constructible_v<array<int, 3>>);
        static_assert(
            std::is_trivially_move_constructible_v<array<double, 3>>
        );
        static_assert(
            !std::is_trivially_move_constructible_v<array<std::string, 3>>
        );
        SUCCEED();
    }

    TEST_F(trait_tests, is_structural) {
        // Ignore errors. `std::structural_v` is a C++26 type trait.
        static_assert(std::is_structural_v<array<int, 3>>);
        static_assert(std::is_structural_v<array<double, 3>>);
        static_assert(!std::is_structural_v<array<std::string, 3>>);

        SUCCEED();
    }

    TEST_F(trait_tests, is_destructible) {
        static_assert(std::is_destructible_v<array<int, 3>>);
        static_assert(std::is_destructible_v<array<double, 3>>);
        static_assert(std::is_destructible_v<array<std::string, 3>>);

        SUCCEED();
    }

    TEST_F(trait_tests, is_trivially_destructible) {
        static_assert(std::is_trivially_destructible_v<array<int, 3>>);
        static_assert(std::is_trivially_destructible_v<array<double, 3>>);
        static_assert(
            !std::is_trivially_destructible_v<array<std::string, 3>>
        );

        SUCCEED();
    }

    TEST_F(trait_tests, is_copy_assignable) {
        static_assert(std::is_copy_assignable_v<array<int, 3>>);
        static_assert(std::is_copy_assignable_v<array<double, 3>>);
        static_assert(std::is_copy_assignable_v<array<std::string, 3>>);
        static_assert(
            !std::is_copy_assignable_v<array<std::unique_ptr<int>, 3>>
        );

        SUCCEED();
    }

    TEST_F(trait_tests, is_trivially_copy_assignable) {
        static_assert(std::is_trivially_copy_assignable_v<array<int, 3>>);
        static_assert(std::is_trivially_copy_assignable_v<array<double, 3>>);
        static_assert(
            !std::is_trivially_copy_assignable_v<array<std::string, 3>>
        );
        static_assert(
            !std::is_trivially_copy_assignable_v<
                array<std::unique_ptr<int>, 3>
            >
        );

        SUCCEED();
    }

    TEST_F(trait_tests, is_move_assignable) {
        static_assert(std::is_move_assignable_v<array<int, 3>>);
        static_assert(std::is_move_assignable_v<array<double, 3>>);
        static_assert(std::is_move_assignable_v<array<std::string, 3>>);

        SUCCEED();
    }

    TEST_F(trait_tests, is_trivially_move_assignable) {
        static_assert(std::is_trivially_move_assignable_v<array<int, 3>>);
        static_assert(std::is_trivially_move_assignable_v<array<double, 3>>);
        static_assert(
            !std::is_trivially_move_assignable_v<array<std::string, 3>>
        );
        static_assert(
            !std::is_trivially_move_assignable_v<
                array<std::unique_ptr<int>, 3>
            >
        );

        SUCCEED();
    }

    TEST_F(trait_tests, is_standard_layout) {
        static_assert(std::is_standard_layout_v<array<int, 3>>);
        static_assert(sizeof(array<int, 3>) == sizeof(int[3]));
        static_assert(alignof(array<int, 3>) == alignof(int[3]));

        static_assert(std::is_standard_layout_v<array<double, 3>>);
        static_assert(sizeof(array<int, 3>) == sizeof(int[3]));
        static_assert(alignof(array<double, 3>) == alignof(double[3]));

        static_assert(std::is_standard_layout_v<array<std::string, 3>>);
        static_assert(sizeof(array<std::string, 3>) == sizeof(std::string[3]));
        static_assert(
            alignof(array<std::string, 3>) == alignof(std::string[3])
        );

        SUCCEED();
    }

    TEST_F(trait_tests, is_deducible) {
        static_assert(deducible<int, int, int>);
        static_assert(deducible<double, double, double>);
        static_assert(deducible<std::string, std::string, std::string>);

        static_assert(!deducible<int, double, std::string>);
        static_assert(!deducible<int, int, double>);
        static_assert(!deducible<int, int, std::string>);
    }

} // namespace collections::array_testing
