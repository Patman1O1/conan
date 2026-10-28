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
    TEST_F(operator_tests, equals_operator_self_compare) {
        constexpr array ints = {1, 2, 3};
        static_assert(ints == ints);
        EXPECT_TRUE(ints == ints);

        const array<std::string, 3> strs = {"This", "is a", "test"};
        EXPECT_TRUE(strs == strs);
    }

    TEST_F(operator_tests, equals_operator_true_case) {
        constexpr array ints_lhs = {1, 2, 3};
        constexpr array ints_rhs = {1, 2, 3};

        static_assert(ints_lhs == ints_rhs);

        EXPECT_TRUE(ints_lhs == ints_rhs);

        const array<std::string, 3> strs_lhs = {"This", "is a", "test"};
        const array<std::string, 3> strs_rhs = {"This", "is a", "test"};

        EXPECT_TRUE(strs_lhs == strs_rhs);
    }

    TEST_F(operator_tests, equals_operator_false_case) {
        constexpr array ints_lhs = {1, 2, 3};
        constexpr array ints_rhs = {4, 5, 6};

        static_assert(!(ints_lhs == ints_rhs));

        EXPECT_FALSE(ints_lhs == ints_rhs);

        const array<std::string, 3> strs_lhs = {"This", "is a", "test"};
        const array<std::string, 3> strs_rhs = {
            "This", "is a different", "test"
        };

        EXPECT_FALSE(strs_lhs == strs_rhs);
    }

    TEST_F(operator_tests, not_equals_operator_true_case) {
        constexpr array ints_lhs = {1, 2, 3};
        constexpr array ints_rhs = {4, 5, 6};

        static_assert(ints_lhs != ints_rhs);

        EXPECT_TRUE(ints_lhs != ints_rhs);

        const array<std::string, 3> strs_lhs = {"This", "is a", "test"};
        const array<std::string, 3> strs_rhs = {
            "This", "is a different", "test"
        };

        EXPECT_TRUE(strs_lhs != strs_rhs);
    }

    TEST_F(operator_tests, not_equals_operator_false_case) {
        constexpr array ints_lhs = {1, 2, 3};
        constexpr array ints_rhs = {1, 2, 3};

        static_assert(!(ints_lhs != ints_rhs));

        EXPECT_FALSE(ints_lhs != ints_rhs);

        const array<std::string, 3> strs_lhs = {"This", "is a", "test"};
        const array<std::string, 3> strs_rhs = {"This", "is a", "test"};

        EXPECT_FALSE(strs_lhs != strs_rhs);
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
