// ISO C Includes
#include <cstddef>
#include <cstdint>

// ISO C++ Includes
#include <algorithm>
#include <concepts>
#include <expected>
#include <functional>
#include <iterator>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <system_error>
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

        class iterator_tests : public testing::Test {};
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

    TEST_F(operator_tests, subscript_operator__mut_overload__returns_mut_ref) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>()[0]),
                array<int, 3>::reference
            >
        );

        static_assert(
            !std::is_const_v<
                std::remove_reference_t<
                    decltype(std::declval<array<int, 3>&>()[0])
                >
            >
        );

        SUCCEED();
    }

    TEST_F(
        operator_tests,
        subscript_operator__mut_overload__is_noexcept
    ) {
        static_assert(
            noexcept(std::declval<array<int, 3>&>()[0])
        );

        SUCCEED();
    }

    TEST_F(
        operator_tests,
        subscript_operator__mut_overload__compile_time
    ) {
        static_assert([] -> bool {
            array values = {1, 2, 3};
            values[0] = 10;
            return 10 == values[0] && 2 == values[1] && 3 == values[2];
        }());

        SUCCEED();
    }

    TEST_F(
        operator_tests,
        subscript_operator__mut_overload__aliases_storage
    ) {
        array values = {1, 2, 3};

        const int& first  = values[0];
        int& second = values[1];

        EXPECT_EQ(1, &second - &first);

        second = 20;
        EXPECT_EQ(20, values[1]);
    }

    TEST_F(
        operator_tests,
        subscript_operator__const_overload__returns_const_ref
    ) {
        static_assert(
            std::same_as<
                decltype(std::declval<const array<int, 3>&>()[0]),
                array<int, 3>::const_reference
            >
        );

        static_assert(
            std::is_const_v<
                std::remove_reference_t<
                    decltype(
                        std::declval<const array<int, 3>&>()[0]
                    )
                >
            >
        );

        SUCCEED();
    }

    TEST_F(
        operator_tests,
        subscript_operator__const_overload__is_noexcept
    ) {
        static_assert(
            noexcept(std::declval<const array<int, 3>&>()[0])
        );

        SUCCEED();
    }

    TEST_F(
        operator_tests,
        subscript_operator__const_overload__compile_time
    ) {
        constexpr array values = {1, 2, 3};

        static_assert(1 == values[0]);
        static_assert(2 == values[1]);
        static_assert(3 == values[2]);

        SUCCEED();
    }

    TEST_F(
        operator_tests,
        subscript_operator__const_overload__aliases_storage
    ) {
        constexpr array values = {1, 2, 3};

        const int& first  = values[0];
        const int& second = values[1];

        EXPECT_EQ(1, &second - &first);
    }

    TEST_F(
        operator_tests,
        subscript_operator__const_overload__does_not_copy_non_trivial
    ) {
        const array values = {
            std::string("alpha"), std::string("beta")
        };

        EXPECT_EQ("alpha", values[0]);
        EXPECT_EQ(values.data(), values.data());
    }

    // ── Method Tests ────────────────────────────────────────────────────────
    TEST_F(method_tests, at__mut_overload__returns_mut_ref) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().at(0)),
                array<int, 3>::reference
            >
        );

        static_assert(
            !std::is_const_v<
                std::remove_reference_t<
                    decltype(std::declval<array<int, 3>&>().at(0))
                >
            >
        );

        SUCCEED();
    }

    TEST_F(method_tests, at__mut_overload__is_not_noexcept) {
        static_assert(
            !noexcept(std::declval<array<int, 3>&>().at(0))
        );

        SUCCEED();
    }

    TEST_F(method_tests, at__mut_overload__valid_indices) {
        array values = {10, 20, 30};

        EXPECT_EQ(10, values.at(0));
        EXPECT_EQ(20, values.at(1));
        EXPECT_EQ(30, values.at(2));
    }

    TEST_F(method_tests, at__mut_overload__invalid_indices) {
        array values = {10, 20, 30};

        EXPECT_THROW(
            static_cast<void>(values.at(static_cast<std::size_t>(-1))),
            std::out_of_range
        );
        EXPECT_THROW(static_cast<void>(values.at(3)), std::out_of_range);
    }

    TEST_F(method_tests, at__mut_overload__is_mutable) {
        static_assert([] -> bool {
            array values = {1, 2, 3};
            values.at(0) = 10;
            return 10 == values.at(0) &&
                   2 == values.at(1) &&
                   3 == values.at(2);
        }());

        array values = {10, 20, 30};

        values.at(0) = 100;
        EXPECT_EQ(100, values[0]);

        values.at(2) = 300;
        EXPECT_EQ(300, values[2]);
    }

    TEST_F(method_tests, at__mut_overload__matches_subscript) {
        array values = {10, 20, 30};

        EXPECT_EQ(values.at(0), values[0]);
        EXPECT_EQ(values.at(1), values[1]);
        EXPECT_EQ(values.at(2), values[2]);
    }


    TEST_F(method_tests, at__const_overload__returns_const_ref) {
        static_assert(
            std::same_as<
                decltype(std::declval<const array<int, 3>&>().at(0)),
                array<int, 3>::const_reference
            >
        );

        static_assert(
            std::is_const_v<
                std::remove_reference_t<
                    decltype(std::declval<const array<int, 3>&>().at(0))
                >
            >
        );

        SUCCEED();
    }

    TEST_F(method_tests, at__const_overload__is_not_noexcept) {
        static_assert(
            !noexcept(std::declval<const array<int, 3>&>().at(0))
        );

        SUCCEED();
    }


    TEST_F(method_tests, at__const_overload__matches_subscript) {
        constexpr array values = {10, 20, 30};

        static_assert(values.at(0) == values[0]);
        static_assert(values.at(1) == values[1]);
        static_assert(values.at(2) == values[2]);

        EXPECT_EQ(values.at(0), values[0]);
        EXPECT_EQ(values.at(1), values[1]);
        EXPECT_EQ(values.at(2), values[2]);
    }

    TEST_F(method_tests, expect_at__mut_overload__returns_expected_mut_ref) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().expect_at(0)),
                std::expected<std::reference_wrapper<int>, std::error_code>
            >
        );

        SUCCEED();
    }

    TEST_F(method_tests, expect_at__mut_overload__is_noexcept) {
        static_assert(
            noexcept(std::declval<array<int, 3>&>().expect_at(0))
        );

        SUCCEED();
    }

    TEST_F(method_tests, expect_at__mut_overload__valid_indices) {
        array values = {10, 20, 30};

        ASSERT_TRUE(values.expect_at(0).has_value());
        ASSERT_TRUE(values.expect_at(1).has_value());
        ASSERT_TRUE(values.expect_at(2).has_value());

        EXPECT_EQ(10, values.expect_at(0)->get());
        EXPECT_EQ(20, values.expect_at(1)->get());
        EXPECT_EQ(30, values.expect_at(2)->get());
    }

    TEST_F(method_tests, expect_at__mut_overload__invalid_indices) {
        array values = {10, 20, 30};

        const auto past_end = values.expect_at(3);
        ASSERT_FALSE(past_end.has_value());
        EXPECT_EQ(
            std::make_error_code(std::errc::result_out_of_range),
            past_end.error()
        );

        const auto wrapped = values.expect_at(static_cast<std::size_t>(-1));
        ASSERT_FALSE(wrapped.has_value());
        EXPECT_EQ(
            std::make_error_code(std::errc::result_out_of_range),
            wrapped.error()
        );
    }

    TEST_F(
        method_tests,
        expect_at__mut_overload__invalid_index_does_not_modify
    ) {
        array values = {10, 20, 30};
        const auto before = values;

        static_cast<void>(values.expect_at(3));
        EXPECT_EQ(before, values);
    }

    TEST_F(method_tests, expect_at__mut_overload__is_mutable) {
        static_assert([] -> bool {
            array values = {1, 2, 3};
            values.expect_at(0)->get() = 10;
            return 10 == values.expect_at(0)->get() &&
                   2 == values.expect_at(1)->get() &&
                   3 == values.expect_at(2)->get();
        }());

        array values = {10, 20, 30};

        values.expect_at(0)->get() = 100;
        EXPECT_EQ(100, values[0]);

        values.expect_at(2).value().get() = 300;
        EXPECT_EQ(300, values[2]);

        EXPECT_EQ(20, values[1]);
    }

    TEST_F(method_tests, expect_at__mut_overload__aliases_storage) {
        array values = {10, 20, 30};

        EXPECT_EQ(&values[0], &values.expect_at(0)->get());
        EXPECT_EQ(&values[1], &values.expect_at(1)->get());
        EXPECT_EQ(&values[2], &values.expect_at(2)->get());
    }

    TEST_F(
        method_tests,
        expect_at__const_overload__returns_expected_const_ref
    ) {
        static_assert(
            std::same_as<
                decltype(std::declval<const array<int, 3>&>().at(0)),
                array<int, 3>::const_reference
            >
        );

        static_assert(
            std::is_const_v<
                std::remove_reference_t<
                    decltype(std::declval<const array<int, 3>&>().at(0))
                >
            >
        );

        SUCCEED();
    }

    TEST_F(method_tests, expect_at__const_overload__is_noexcept) {
        static_assert(
            noexcept(std::declval<const array<int, 3>&>().expect_at(0))
        );

        SUCCEED();
    }

    TEST_F(method_tests, expect_at__const_overload__valid_indices) {
        constexpr array values = {10, 20, 30};

        static_assert(values.expect_at(0).has_value());
        static_assert(values.expect_at(1).has_value());
        static_assert(values.expect_at(2).has_value());

        static_assert(values.expect_at(0)->get() == 10);
        static_assert(values.expect_at(1)->get() == 20);
        static_assert(values.expect_at(2)->get() == 30);

        ASSERT_TRUE(values.expect_at(0).has_value());
        ASSERT_TRUE(values.expect_at(1).has_value());
        ASSERT_TRUE(values.expect_at(2).has_value());

        EXPECT_EQ(10, values.expect_at(0)->get());
        EXPECT_EQ(20, values.expect_at(1)->get());
        EXPECT_EQ(30, values.expect_at(2)->get());
    }

    TEST_F(method_tests, expect_at__const_overload__invalid_indices) {
        constexpr array values = {10, 20, 30};

        const auto past_end = values.expect_at(3);
        ASSERT_FALSE(past_end.has_value());
        EXPECT_EQ(
            std::make_error_code(std::errc::result_out_of_range),
            past_end.error()
        );

        const auto wrapped = values.expect_at(static_cast<std::size_t>(-1));
        ASSERT_FALSE(wrapped.has_value());
        EXPECT_EQ(
            std::make_error_code(std::errc::result_out_of_range),
            wrapped.error()
        );
    }

    TEST_F(
        method_tests,
        expect_at__const_overload__invalid_index_does_not_modify
    ) {
        constexpr array values = {10, 20, 30};
        constexpr auto before = values;

        static_cast<void>(values.expect_at(3));

        static_assert(before == values);
        EXPECT_EQ(before, values);
    }

    TEST_F(method_tests, expect_const__mut_overload__aliases_storage) {
        constexpr array values = {10, 20, 30};

        static_assert(values.data() == &values.expect_at(0)->get());
        static_assert(&values[1] == &values.expect_at(1)->get());
        static_assert(&values[2] == &values.expect_at(2)->get());

        EXPECT_EQ(values.data(), &values.expect_at(0)->get());
        EXPECT_EQ(&values[1], &values.expect_at(1)->get());
        EXPECT_EQ(&values[2], &values.expect_at(2)->get());
    }

    TEST_F(method_tests, front__mut_overload__single_element) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 1>&>().front()),
                array<int, 1>::reference
            >
        );

        static_assert(noexcept(std::declval<array<int, 1>&>().front()));

        static_assert([] -> bool {
            array values = {1};
            values.front() = 10;
            return 10 == values.front() && 10 == values[0];
        }());

        array values = {1};
        EXPECT_EQ(1, values.front());
        EXPECT_EQ(values.data(), &values.front());
        EXPECT_EQ(&values.back(), &values.front());

        values.front() = 10;
        EXPECT_EQ(10, values[0]);
    }

    TEST_F(method_tests, front__mut_overload__multiple_elements) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().front()),
                array<int, 3>::reference
            >
        );

        static_assert(noexcept(std::declval<array<int, 3>&>().front()));

        static_assert([] -> bool {
            array values = {1, 2, 3};
            values.front() = 10;
            return 10 == values.front() && 10 == values[0] &&
                   2 == values[1] && 3 == values[2];
        }());

        array values = {1, 2, 3};
        EXPECT_EQ(1, values.front());
        EXPECT_EQ(values.data(), &values.front());
        EXPECT_NE(&values.back(), &values.front());

        values.front() = 10;
        EXPECT_EQ(10, values[0]);
        EXPECT_EQ(2,  values[1]);
        EXPECT_EQ(3,  values[2]);
    }

    TEST_F(method_tests, front__const_overload__single_element) {
        static_assert(
            std::same_as<
                decltype(std::declval<const array<int, 1>&>().front()),
                array<int, 1>::const_reference
            >
        );

        static_assert(
            std::is_const_v<
                std::remove_reference_t<
                    decltype(std::declval<const array<int, 1>&>().front())
                >
            >
        );

        static_assert(
            noexcept(std::declval<const array<int, 1>&>().front())
        );

        constexpr array values = {1};
        static_assert(1 == values.front());
        static_assert(values.data() == &values.front());
        static_assert(values.front() == values.back());

        EXPECT_EQ(1, values.front());
        EXPECT_EQ(&values[0], &values.front());
        EXPECT_EQ(&values.back(), &values.front());
    }

    TEST_F(method_tests, front__const_overload__multiple_elements) {
        static_assert(
            std::same_as<
                decltype(std::declval<const array<int, 3>&>().front()),
                array<int, 3>::const_reference
            >
        );

        static_assert(
            std::is_const_v<
                std::remove_reference_t<
                    decltype(std::declval<const array<int, 3>&>().front())
                >
            >
        );

        static_assert(
            noexcept(std::declval<const array<int, 3>&>().front())
        );

        constexpr array values = {1, 2, 3};
        static_assert(1 == values.front());
        static_assert(values.front() == values[0]);

        EXPECT_EQ(1, values.front());
        EXPECT_EQ(values.data(), &values.front());
        EXPECT_NE(&values.back(), &values.front());
    }

    TEST_F(method_tests, back__mut_overload__single_element) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 1>&>().back()),
                array<int, 1>::reference
            >
        );

        static_assert(noexcept(std::declval<array<int, 1>&>().back()));

        static_assert([] -> bool {
            array values = {1};
            values.back() = 10;
            return 10 == values.back() && 10 == values.front();
        }());

        array values = {1};
        EXPECT_EQ(1, values.back());
        EXPECT_EQ(values.data(), &values.back());
        EXPECT_EQ(&values.front(), &values.back());

        values.back() = 10;
        EXPECT_EQ(10, values[0]);
    }

    TEST_F(method_tests, back__mut_overload__multiple_elements) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().back()),
                array<int, 3>::reference
            >
        );

        static_assert(noexcept(std::declval<array<int, 3>&>().back()));

        static_assert([] -> bool {
            array values = {1, 2, 3};
            values.back() = 30;
            return 30 == values.back() && 1 == values[0] &&
                   2 == values[1] && 30 == values[2];
        }());

        array values = {1, 2, 3};
        EXPECT_EQ(3, values.back());
        EXPECT_EQ(&values[2], &values.back());
        EXPECT_EQ(2, &values.back() - &values.front());

        values.back() = 30;
        EXPECT_EQ(1,  values[0]);
        EXPECT_EQ(2,  values[1]);
        EXPECT_EQ(30, values[2]);
    }

    TEST_F(method_tests, back__const_overload__single_element) {
        static_assert(
            std::same_as<
                decltype(std::declval<const array<int, 1>&>().back()),
                array<int, 1>::const_reference
            >
        );

        static_assert(
            std::is_const_v<
                std::remove_reference_t<
                    decltype(std::declval<const array<int, 1>&>().back())
                >
            >
        );

        static_assert(noexcept(std::declval<const array<int, 1>&>().back()));

        constexpr array values = {1};
        static_assert(1 == values.back());

        EXPECT_EQ(1, values.back());
        EXPECT_EQ(values.data(), &values.back());
        EXPECT_EQ(&values.front(), &values.back());
    }

    TEST_F(method_tests, back__const_overload__multiple_elements) {
        static_assert(
            std::same_as<
                decltype(std::declval<const array<int, 3>&>().back()),
                array<int, 3>::const_reference
            >
        );

        static_assert(
            std::is_const_v<
                std::remove_reference_t<
                    decltype(std::declval<const array<int, 3>&>().back())
                >
            >
        );

        static_assert(
            noexcept(std::declval<const array<int, 3>&>().back())
        );

        constexpr array values = {1, 2, 3};
        static_assert(3 == values.back());
        static_assert(values.back() == values[2]);

        EXPECT_EQ(3, values.back());
        EXPECT_EQ(&values[2], &values.back());
        EXPECT_EQ(2, &values.back() - &values.front());
    }

    TEST_F(method_tests, data__returns_pointer_to_first_element) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().data()),
                array<int, 3>::pointer
            >
        );

        static_assert(
            std::same_as<
                decltype(std::declval<const array<int, 3>&>().data()),
                array<int, 3>::const_pointer
            >
        );

        static_assert(noexcept(std::declval<array<int, 3>&>().data()));

        static_assert(
            noexcept(std::declval<const array<int, 3>&>().data())
        );

        array values = {1, 2, 3};
        EXPECT_EQ(values.data(), values.data());
        EXPECT_EQ(&values[2], values.data() + 2);
    }

    TEST_F(method_tests, data__const_overload__returns_const_pointer) {
        constexpr array values = {1, 2, 3};
        const int* p = values.data();

        EXPECT_EQ(1, p[0]);
        EXPECT_EQ(2, p[1]);
        EXPECT_EQ(3, p[2]);
    }

    TEST_F(method_tests, data__points_into_values_field) {
        array values = {1, 2, 3};
        EXPECT_EQ(values.data(), values.values);
    }

    TEST_F(method_tests, size) {
        static_assert(3 == collections::array<int, 3>::size());
        static_assert(1 == collections::array<int, 1>::size());
        static_assert(0 == collections::array<int, 0>::size());

        static_assert(
            noexcept(collections::array<int, 3>::size())
        );

        static_assert(
            std::same_as<
                decltype(collections::array<int, 3>::size()),
                array<int, 3>::size_type
            >
        );

        SUCCEED();
    }

    // ── begin() ─────────────────────────────────────────────────────────────
    TEST_F(method_tests, begin__mut_overload__returns_iterator) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().begin()),
                array<int, 3>::iterator
            >
        );

        static_assert(
            !std::is_const_v<
                std::remove_reference_t<
                    decltype(*std::declval<array<int, 3>&>().begin())
                >
            >
        );

        SUCCEED();
    }

    TEST_F(method_tests, begin__mut_overload__is_noexcept) {
        static_assert(noexcept(std::declval<array<int, 3>&>().begin()));

        SUCCEED();
    }

    TEST_F(method_tests, begin__mut_overload__points_to_first_element) {
        static_assert([] -> bool {
            array values = {1, 2, 3};
            return values.data() == values.begin() &&
                   &values.front() == values.begin() &&
                   1 == *values.begin();
        }());

        // Single Element
        array single = {1};
        EXPECT_EQ(single.data(), single.begin());
        EXPECT_EQ(&single.front(), single.begin());
        EXPECT_EQ(1, *single.begin());

        // Multiple Elements
        array multiple = {1, 2, 3};
        EXPECT_EQ(multiple.data(), multiple.begin());
        EXPECT_EQ(&multiple.front(), multiple.begin());
        EXPECT_EQ(1, *multiple.begin());
    }

    TEST_F(method_tests, begin__mut_overload__is_mutable) {
        static_assert([] -> bool {
            array values = {1, 2, 3};
            *values.begin() = 10;
            return 10 == values[0] && 2 == values[1] && 3 == values[2];
        }());

        array values = {1, 2, 3};
        *values.begin() = 10;
        EXPECT_EQ(10, values[0]);
        EXPECT_EQ(2,  values[1]);
        EXPECT_EQ(3,  values[2]);
    }

    TEST_F(method_tests, begin__mut_overload__nontrivial) {
        array<std::string, 3> values = {"This", "is a", "test"};

        EXPECT_EQ("This", *values.begin());
        EXPECT_EQ(4u, values.begin()->size());

        *values.begin() = "That";
        EXPECT_EQ("That", values[0]);
        EXPECT_EQ("is a", values[1]);
        EXPECT_EQ("test", values[2]);
    }

    TEST_F(method_tests, begin__const_overload__returns_const_iterator) {
        static_assert(
            std::same_as<
                decltype(std::declval<const array<int, 3>&>().begin()),
                array<int, 3>::const_iterator
            >
        );

        static_assert(
            std::is_const_v<
                std::remove_reference_t<
                    decltype(*std::declval<const array<int, 3>&>().begin())
                >
            >
        );

        SUCCEED();
    }

    TEST_F(method_tests, begin__const_overload__is_noexcept) {
        static_assert(
            noexcept(std::declval<const array<int, 3>&>().begin())
        );

        SUCCEED();
    }

    TEST_F(method_tests, begin__const_overload__points_to_first_element) {
        // Single Element
        constexpr array single = {1};
        static_assert(single.data() == single.begin());
        static_assert(1 == *single.begin());

        EXPECT_EQ(single.data(), single.begin());
        EXPECT_EQ(&single.front(), single.begin());

        // Multiple Elements
        constexpr array multiple = {1, 2, 3};
        static_assert(multiple.data() == multiple.begin());
        static_assert(1 == *multiple.begin());

        EXPECT_EQ(multiple.data(), multiple.begin());
        EXPECT_EQ(&multiple.front(), multiple.begin());
    }

    // ── end() ───────────────────────────────────────────────────────────────
    TEST_F(method_tests, end__mut_overload__returns_iterator) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().end()),
                array<int, 3>::iterator
            >
        );

        SUCCEED();
    }

    TEST_F(method_tests, end__mut_overload__is_noexcept) {
        static_assert(noexcept(std::declval<array<int, 3>&>().end()));

        SUCCEED();
    }

    TEST_F(method_tests, end__mut_overload__points_one_past_last_element) {
        static_assert([] -> bool {
            array values = {1, 2, 3};
            return values.data() + 3 == values.end() &&
                   &values.back() + 1 == values.end() &&
                   3 == values.end() - values.begin();
        }());

        // Single Element
        array single = {1};
        EXPECT_EQ(single.data() + 1, single.end());
        EXPECT_EQ(&single.back() + 1, single.end());
        EXPECT_EQ(1, single.end() - single.begin());

        // Multiple Elements
        array multiple = {1, 2, 3};
        EXPECT_EQ(multiple.data() + 3, multiple.end());
        EXPECT_EQ(&multiple.back() + 1, multiple.end());
        EXPECT_EQ(3, multiple.end() - multiple.begin());
    }

    TEST_F(method_tests, end__mut_overload__is_mutable) {
        static_assert([] -> bool {
            array values = {1, 2, 3};
            *(values.end() - 1) = 30;
            return 1 == values[0] && 2 == values[1] && 30 == values[2];
        }());

        array values = {1, 2, 3};
        *(values.end() - 1) = 30;
        EXPECT_EQ(1,  values[0]);
        EXPECT_EQ(2,  values[1]);
        EXPECT_EQ(30, values[2]);
    }

    TEST_F(method_tests, end__mut_overload__bounds_full_traversal) {
        static_assert([] -> bool {
            array values = {1, 2, 3};
            for (auto it = values.begin(); it != values.end(); ++it) {
                *it *= 10;
            }
            return 10 == values[0] && 20 == values[1] && 30 == values[2];
        }());

        array values = {1, 2, 3};
        std::size_t visited = 0;
        for (auto it = values.begin(); it != values.end(); ++it) {
            EXPECT_EQ(&values[visited], it);
            ++visited;
        }
        EXPECT_EQ(values.size(), visited);
    }

    TEST_F(method_tests, end__const_overload__returns_const_iterator) {
        static_assert(
            std::same_as<
                decltype(std::declval<const array<int, 3>&>().end()),
                array<int, 3>::const_iterator
            >
        );

        SUCCEED();
    }

    TEST_F(method_tests, end__const_overload__is_noexcept) {
        static_assert(noexcept(std::declval<const array<int, 3>&>().end()));

        SUCCEED();
    }

    TEST_F(method_tests, end__const_overload__points_one_past_last_element) {
        // Single Element
        constexpr array single = {1};
        static_assert(single.data() + 1 == single.end());
        static_assert(1 == single.end() - single.begin());

        EXPECT_EQ(&single.back() + 1, single.end());

        // Multiple Elements
        constexpr array multiple = {1, 2, 3};
        static_assert(multiple.data() + 3 == multiple.end());
        static_assert(3 == multiple.end() - multiple.begin());
        static_assert(3 == *(multiple.end() - 1));

        EXPECT_EQ(&multiple.back() + 1, multiple.end());
        EXPECT_EQ(3, *(multiple.end() - 1));
    }

    // ── cbegin() / cend() ───────────────────────────────────────────────────
    TEST_F(method_tests, cbegin__returns_const_iterator) {
        // Must yield a const_iterator even when called on a mutable array.
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().cbegin()),
                array<int, 3>::const_iterator
            >
        );

        static_assert(
            std::same_as<
                decltype(std::declval<const array<int, 3>&>().cbegin()),
                array<int, 3>::const_iterator
            >
        );

        static_assert(
            std::is_const_v<
                std::remove_reference_t<
                    decltype(*std::declval<array<int, 3>&>().cbegin())
                >
            >
        );

        SUCCEED();
    }

    TEST_F(method_tests, cbegin__is_noexcept) {
        static_assert(noexcept(std::declval<array<int, 3>&>().cbegin()));

        SUCCEED();
    }

    TEST_F(method_tests, cbegin__matches_begin) {
        constexpr array values = {1, 2, 3};
        static_assert(values.begin() == values.cbegin());
        static_assert(1 == *values.cbegin());

        array mutable_values = {1, 2, 3};
        EXPECT_EQ(mutable_values.begin(), mutable_values.cbegin());
        EXPECT_EQ(mutable_values.data(), mutable_values.cbegin());
    }

    TEST_F(method_tests, cend__returns_const_iterator) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().cend()),
                array<int, 3>::const_iterator
            >
        );

        static_assert(
            std::same_as<
                decltype(std::declval<const array<int, 3>&>().cend()),
                array<int, 3>::const_iterator
            >
        );

        SUCCEED();
    }

    TEST_F(method_tests, cend__is_noexcept) {
        static_assert(noexcept(std::declval<array<int, 3>&>().cend()));

        SUCCEED();
    }

    TEST_F(method_tests, cend__matches_end) {
        constexpr array values = {1, 2, 3};
        static_assert(values.end() == values.cend());
        static_assert(3 == values.cend() - values.cbegin());

        array mutable_values = {1, 2, 3};
        EXPECT_EQ(mutable_values.end(), mutable_values.cend());
        EXPECT_EQ(mutable_values.data() + 3, mutable_values.cend());
    }

    // ── rbegin() / rend() ───────────────────────────────────────────────────
    TEST_F(method_tests, rbegin__mut_overload__returns_reverse_iterator) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().rbegin()),
                array<int, 3>::reverse_iterator
            >
        );

        static_assert(
            !std::is_const_v<
                std::remove_reference_t<
                    decltype(*std::declval<array<int, 3>&>().rbegin())
                >
            >
        );

        SUCCEED();
    }

    TEST_F(method_tests, rbegin__mut_overload__is_noexcept) {
        static_assert(noexcept(std::declval<array<int, 3>&>().rbegin()));

        SUCCEED();
    }

    TEST_F(method_tests, rbegin__mut_overload__points_to_last_element) {
        static_assert([] -> bool {
            array values = {1, 2, 3};
            return values.end() == values.rbegin().base() &&
                   &values.back() == &*values.rbegin() &&
                   3 == *values.rbegin();
        }());

        // Single Element
        array single = {1};
        EXPECT_EQ(single.end(), single.rbegin().base());
        EXPECT_EQ(&single.back(), &*single.rbegin());
        EXPECT_EQ(&single.front(), &*single.rbegin());

        // Multiple Elements
        array multiple = {1, 2, 3};
        EXPECT_EQ(multiple.end(), multiple.rbegin().base());
        EXPECT_EQ(&multiple.back(), &*multiple.rbegin());
        EXPECT_EQ(3, *multiple.rbegin());
    }

    TEST_F(method_tests, rbegin__mut_overload__is_mutable) {
        static_assert([] -> bool {
            array values = {1, 2, 3};
            *values.rbegin() = 30;
            return 1 == values[0] && 2 == values[1] && 30 == values[2];
        }());

        array values = {1, 2, 3};
        *values.rbegin() = 30;
        EXPECT_EQ(1,  values[0]);
        EXPECT_EQ(2,  values[1]);
        EXPECT_EQ(30, values[2]);
    }

    TEST_F(method_tests, rend__mut_overload__returns_reverse_iterator) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().rend()),
                array<int, 3>::reverse_iterator
            >
        );

        SUCCEED();
    }

    TEST_F(method_tests, rend__mut_overload__is_noexcept) {
        static_assert(noexcept(std::declval<array<int, 3>&>().rend()));

        SUCCEED();
    }

    TEST_F(method_tests, rend__mut_overload__points_one_before_first_element) {
        static_assert([] -> bool {
            array values = {1, 2, 3};
            return values.begin() == values.rend().base() &&
                   &values.front() == &*(values.rend() - 1) &&
                   3 == values.rend() - values.rbegin();
        }());

        // Single Element
        array single = {1};
        EXPECT_EQ(single.begin(), single.rend().base());
        EXPECT_EQ(1, single.rend() - single.rbegin());

        // Multiple Elements
        array multiple = {1, 2, 3};
        EXPECT_EQ(multiple.begin(), multiple.rend().base());
        EXPECT_EQ(&multiple.front(), &*(multiple.rend() - 1));
        EXPECT_EQ(3, multiple.rend() - multiple.rbegin());
    }

    TEST_F(method_tests, rbegin_rend__mut_overload__traverse_in_reverse) {
        static_assert([] -> bool {
            array values = {1, 2, 3};
            int expected = 3;
            for (auto it = values.rbegin(); it != values.rend(); ++it) {
                if (*it != expected--) {
                    return false;
                }
            }
            return 0 == expected;
        }());

        array values = {1, 2, 3};
        constexpr array reversed = {3, 2, 1};
        EXPECT_TRUE(
            std::equal(values.rbegin(), values.rend(), reversed.begin())
        );

        std::fill(values.rbegin(), values.rbegin() + 2, 0);
        EXPECT_EQ(1, values[0]);
        EXPECT_EQ(0, values[1]);
        EXPECT_EQ(0, values[2]);
    }

    TEST_F(
        method_tests,
        rbegin__const_overload__returns_const_reverse_iterator
    ) {
        static_assert(
            std::same_as<
                decltype(std::declval<const array<int, 3>&>().rbegin()),
                array<int, 3>::const_reverse_iterator
            >
        );

        static_assert(
            std::is_const_v<
                std::remove_reference_t<
                    decltype(*std::declval<const array<int, 3>&>().rbegin())
                >
            >
        );

        SUCCEED();
    }

    TEST_F(method_tests, rbegin__const_overload__is_noexcept) {
        static_assert(
            noexcept(std::declval<const array<int, 3>&>().rbegin())
        );

        SUCCEED();
    }

    TEST_F(method_tests, rbegin__const_overload__points_to_last_element) {
        constexpr array values = {1, 2, 3};
        static_assert(values.end() == values.rbegin().base());
        static_assert(3 == *values.rbegin());
        static_assert(&values.back() == &*values.rbegin());

        EXPECT_EQ(values.end(), values.rbegin().base());
        EXPECT_EQ(&values.back(), &*values.rbegin());
    }

    TEST_F(
        method_tests,
        rend__const_overload__returns_const_reverse_iterator
    ) {
        static_assert(
            std::same_as<
                decltype(std::declval<const array<int, 3>&>().rend()),
                array<int, 3>::const_reverse_iterator
            >
        );

        SUCCEED();
    }

    TEST_F(method_tests, rend__const_overload__is_noexcept) {
        static_assert(noexcept(std::declval<const array<int, 3>&>().rend()));

        SUCCEED();
    }

    TEST_F(
        method_tests,
        rend__const_overload__points_one_before_first_element
    ) {
        constexpr array values = {1, 2, 3};
        static_assert(values.begin() == values.rend().base());
        static_assert(1 == *(values.rend() - 1));
        static_assert(3 == values.rend() - values.rbegin());

        EXPECT_EQ(values.begin(), values.rend().base());
        EXPECT_EQ(&values.front(), &*(values.rend() - 1));
    }

    TEST_F(method_tests, rbegin_rend__const_overload__traverse_in_reverse) {
        constexpr array values = {1, 2, 3};
        constexpr array reversed = {3, 2, 1};

        static_assert(
            std::equal(values.rbegin(), values.rend(), reversed.begin())
        );
        EXPECT_TRUE(
            std::equal(values.rbegin(), values.rend(), reversed.begin())
        );
    }

    // ── crbegin() / crend() ─────────────────────────────────────────────────
    TEST_F(method_tests, crbegin__returns_const_reverse_iterator) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().crbegin()),
                array<int, 3>::const_reverse_iterator
            >
        );

        static_assert(
            std::same_as<
                decltype(std::declval<const array<int, 3>&>().crbegin()),
                array<int, 3>::const_reverse_iterator
            >
        );

        static_assert(
            std::is_const_v<
                std::remove_reference_t<
                    decltype(*std::declval<array<int, 3>&>().crbegin())
                >
            >
        );

        SUCCEED();
    }

    TEST_F(method_tests, crbegin__is_noexcept) {
        static_assert(noexcept(std::declval<array<int, 3>&>().crbegin()));

        SUCCEED();
    }

    TEST_F(method_tests, crbegin__matches_rbegin) {
        constexpr array values = {1, 2, 3};
        static_assert(values.rbegin() == values.crbegin());
        static_assert(values.cend() == values.crbegin().base());
        static_assert(3 == *values.crbegin());

        array mutable_values = {1, 2, 3};
        EXPECT_EQ(mutable_values.cend(), mutable_values.crbegin().base());
        EXPECT_EQ(&mutable_values.back(), &*mutable_values.crbegin());
    }

    TEST_F(method_tests, crend__returns_const_reverse_iterator) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().crend()),
                array<int, 3>::const_reverse_iterator
            >
        );

        static_assert(
            std::same_as<
                decltype(std::declval<const array<int, 3>&>().crend()),
                array<int, 3>::const_reverse_iterator
            >
        );

        SUCCEED();
    }

    TEST_F(method_tests, crend__is_noexcept) {
        static_assert(noexcept(std::declval<array<int, 3>&>().crend()));

        SUCCEED();
    }

    TEST_F(method_tests, crend__matches_rend) {
        constexpr array values = {1, 2, 3};
        static_assert(values.rend() == values.crend());
        static_assert(values.cbegin() == values.crend().base());
        static_assert(3 == values.crend() - values.crbegin());

        array mutable_values = {1, 2, 3};
        EXPECT_EQ(mutable_values.cbegin(), mutable_values.crend().base());
        EXPECT_EQ(3, mutable_values.crend() - mutable_values.crbegin());
    }

    // ── checked_begin() / checked_end() ─────────────────────────────────────
    TEST_F(method_tests, checked_begin__mut_overload__returns_optional) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().checked_begin()),
                std::optional<array<int, 3>::iterator>
            >
        );

        static_assert(
            noexcept(std::declval<array<int, 3>&>().checked_begin())
        );

        SUCCEED();
    }

    TEST_F(method_tests, checked_begin__mut_overload__non_empty) {
        static_assert([] -> bool {
            array values = {1, 2, 3};
            const auto it = values.checked_begin();
            return it.has_value() && values.begin() == *it;
        }());

        array values = {1, 2, 3};
        const auto it = values.checked_begin();
        ASSERT_TRUE(it.has_value());
        EXPECT_EQ(values.begin(), *it);

        **it = 10;
        EXPECT_EQ(10, values[0]);
    }

    TEST_F(method_tests, checked_begin__mut_overload__empty) {
        array<int, 0> values{};
        EXPECT_FALSE(values.checked_begin().has_value());
    }

    TEST_F(method_tests, checked_begin__const_overload__returns_optional) {
        static_assert(
            std::same_as<
                decltype(
                    std::declval<const array<int, 3>&>().checked_begin()
                ),
                std::optional<array<int, 3>::const_iterator>
            >
        );

        static_assert(
            noexcept(std::declval<const array<int, 3>&>().checked_begin())
        );

        SUCCEED();
    }

    TEST_F(method_tests, checked_begin__const_overload__non_empty) {
        constexpr array values = {1, 2, 3};
        static_assert(values.checked_begin().has_value());
        static_assert(values.begin() == *values.checked_begin());

        ASSERT_TRUE(values.checked_begin().has_value());
        EXPECT_EQ(values.begin(), *values.checked_begin());
    }

    TEST_F(method_tests, checked_begin__const_overload__empty) {
        const array<int, 0> values{};
        EXPECT_FALSE(values.checked_begin().has_value());
    }

    TEST_F(method_tests, checked_end__mut_overload__returns_optional) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().checked_end()),
                std::optional<array<int, 3>::iterator>
            >
        );

        static_assert(
            noexcept(std::declval<array<int, 3>&>().checked_end())
        );

        SUCCEED();
    }

    TEST_F(method_tests, checked_end__mut_overload__non_empty) {
        static_assert([] -> bool {
            array values = {1, 2, 3};
            const auto it = values.checked_end();
            return it.has_value() && values.end() == *it;
        }());

        array values = {1, 2, 3};
        const auto it = values.checked_end();
        ASSERT_TRUE(it.has_value());
        EXPECT_EQ(values.end(), *it);
    }

    TEST_F(method_tests, checked_end__mut_overload__empty) {
        array<int, 0> values{};
        EXPECT_FALSE(values.checked_end().has_value());
    }

    TEST_F(method_tests, checked_end__const_overload__returns_optional) {
        static_assert(
            std::same_as<
                decltype(std::declval<const array<int, 3>&>().checked_end()),
                std::optional<array<int, 3>::const_iterator>
            >
        );

        static_assert(
            noexcept(std::declval<const array<int, 3>&>().checked_end())
        );

        SUCCEED();
    }

    TEST_F(method_tests, checked_end__const_overload__non_empty) {
        constexpr array values = {1, 2, 3};
        static_assert(values.checked_end().has_value());
        static_assert(values.end() == *values.checked_end());

        ASSERT_TRUE(values.checked_end().has_value());
        EXPECT_EQ(values.end(), *values.checked_end());
    }

    TEST_F(method_tests, checked_end__const_overload__empty) {
        constexpr array<int, 0> values{};
        EXPECT_FALSE(values.checked_end().has_value());
    }

    // ── checked_cbegin() / checked_cend() ───────────────────────────────────
    TEST_F(method_tests, checked_cbegin__returns_optional) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().checked_cbegin()),
                std::optional<array<int, 3>::const_iterator>
            >
        );

        static_assert(
            noexcept(std::declval<array<int, 3>&>().checked_cbegin())
        );

        SUCCEED();
    }

    TEST_F(method_tests, checked_cbegin__non_empty) {
        constexpr array values = {1, 2, 3};
        static_assert(values.checked_cbegin().has_value());
        static_assert(values.cbegin() == *values.checked_cbegin());

        array mutable_values = {1, 2, 3};
        ASSERT_TRUE(mutable_values.checked_cbegin().has_value());
        EXPECT_EQ(mutable_values.cbegin(), *mutable_values.checked_cbegin());
    }

    TEST_F(method_tests, checked_cbegin__empty) {
        array<int, 0> values{};
        EXPECT_FALSE(values.checked_cbegin().has_value());
    }

    TEST_F(method_tests, checked_cend__returns_optional) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().checked_cend()),
                std::optional<array<int, 3>::const_iterator>
            >
        );

        static_assert(
            noexcept(std::declval<array<int, 3>&>().checked_cend())
        );

        SUCCEED();
    }

    TEST_F(method_tests, checked_cend__non_empty) {
        constexpr array values = {1, 2, 3};
        static_assert(values.checked_cend().has_value());
        static_assert(values.cend() == *values.checked_cend());

        array mutable_values = {1, 2, 3};
        ASSERT_TRUE(mutable_values.checked_cend().has_value());
        EXPECT_EQ(mutable_values.cend(), *mutable_values.checked_cend());
    }

    TEST_F(method_tests, checked_cend__empty) {
        array<int, 0> values{};
        EXPECT_FALSE(values.checked_cend().has_value());
    }

    // ── checked_rbegin() / checked_rend() ───────────────────────────────────
    TEST_F(method_tests, checked_rbegin__mut_overload__returns_optional) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().checked_rbegin()),
                std::optional<array<int, 3>::reverse_iterator>
            >
        );

        static_assert(
            noexcept(std::declval<array<int, 3>&>().checked_rbegin())
        );

        SUCCEED();
    }

    TEST_F(method_tests, checked_rbegin__mut_overload__non_empty) {
        static_assert([] -> bool {
            array values = {1, 2, 3};
            const auto it = values.checked_rbegin();
            return it.has_value() && values.rbegin() == *it && 3 == **it;
        }());

        array values = {1, 2, 3};
        const auto it = values.checked_rbegin();
        ASSERT_TRUE(it.has_value());
        EXPECT_EQ(values.rbegin(), *it);

        **it = 30;
        EXPECT_EQ(30, values[2]);
    }

    TEST_F(method_tests, checked_rbegin__mut_overload__empty) {
        array<int, 0> values{};
        EXPECT_FALSE(values.checked_rbegin().has_value());
    }

    TEST_F(method_tests, checked_rbegin__const_overload__returns_optional) {
        static_assert(
            std::same_as<
                decltype(
                    std::declval<const array<int, 3>&>().checked_rbegin()
                ),
                std::optional<array<int, 3>::const_reverse_iterator>
            >
        );

        static_assert(
            noexcept(std::declval<const array<int, 3>&>().checked_rbegin())
        );

        SUCCEED();
    }

    TEST_F(method_tests, checked_rbegin__const_overload__non_empty) {
        constexpr array values = {1, 2, 3};
        static_assert(values.checked_rbegin().has_value());
        static_assert(values.rbegin() == *values.checked_rbegin());
        static_assert(3 == **values.checked_rbegin());

        ASSERT_TRUE(values.checked_rbegin().has_value());
        EXPECT_EQ(values.rbegin(), *values.checked_rbegin());
    }

    TEST_F(method_tests, checked_rbegin__const_overload__empty) {
        const array<int, 0> values{};
        EXPECT_FALSE(values.checked_rbegin().has_value());
    }

    TEST_F(method_tests, checked_rend__mut_overload__returns_optional) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().checked_rend()),
                std::optional<array<int, 3>::reverse_iterator>
            >
        );

        static_assert(
            noexcept(std::declval<array<int, 3>&>().checked_rend())
        );

        SUCCEED();
    }

    TEST_F(method_tests, checked_rend__mut_overload__non_empty) {
        static_assert([] -> bool {
            array values = {1, 2, 3};
            const auto it = values.checked_rend();
            return it.has_value() && values.rend() == *it;
        }());

        array values = {1, 2, 3};
        const auto it = values.checked_rend();
        ASSERT_TRUE(it.has_value());
        EXPECT_EQ(values.rend(), *it);
    }

    TEST_F(method_tests, checked_rend__mut_overload__empty) {
        array<int, 0> values{};
        EXPECT_FALSE(values.checked_rend().has_value());
    }

    TEST_F(method_tests, checked_rend__const_overload__returns_optional) {
        static_assert(
            std::same_as<
                decltype(
                    std::declval<const array<int, 3>&>().checked_rend()
                ),
                std::optional<array<int, 3>::const_reverse_iterator>
            >
        );

        static_assert(
            noexcept(std::declval<const array<int, 3>&>().checked_rend())
        );

        SUCCEED();
    }

    TEST_F(method_tests, checked_rend__const_overload__non_empty) {
        constexpr array values = {1, 2, 3};
        static_assert(values.checked_rend().has_value());
        static_assert(values.rend() == *values.checked_rend());

        ASSERT_TRUE(values.checked_rend().has_value());
        EXPECT_EQ(values.rend(), *values.checked_rend());
    }

    TEST_F(method_tests, checked_rend__const_overload__empty) {
        const array<int, 0> values{};
        EXPECT_FALSE(values.checked_rend().has_value());
    }

    // ── checked_crbegin() / checked_crend() ─────────────────────────────────
    TEST_F(method_tests, checked_crbegin__returns_optional) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().checked_crbegin()),
                std::optional<array<int, 3>::const_reverse_iterator>
            >
        );

        static_assert(
            noexcept(std::declval<array<int, 3>&>().checked_crbegin())
        );

        SUCCEED();
    }

    TEST_F(method_tests, checked_crbegin__non_empty) {
        constexpr array values = {1, 2, 3};
        static_assert(values.checked_crbegin().has_value());
        static_assert(values.crbegin() == *values.checked_crbegin());

        array mutable_values = {1, 2, 3};
        ASSERT_TRUE(mutable_values.checked_crbegin().has_value());
        EXPECT_EQ(
            mutable_values.crbegin(),
            *mutable_values.checked_crbegin()
        );
    }

    TEST_F(method_tests, checked_crbegin__empty) {
        array<int, 0> values{};
        EXPECT_FALSE(values.checked_crbegin().has_value());
    }

    TEST_F(method_tests, checked_crend__returns_optional) {
        static_assert(
            std::same_as<
                decltype(std::declval<array<int, 3>&>().checked_crend()),
                std::optional<array<int, 3>::const_reverse_iterator>
            >
        );

        static_assert(
            noexcept(std::declval<array<int, 3>&>().checked_crend())
        );

        SUCCEED();
    }

    TEST_F(method_tests, checked_crend__non_empty) {
        constexpr array values = {1, 2, 3};
        static_assert(values.checked_crend().has_value());
        static_assert(values.crend() == *values.checked_crend());

        array mutable_values = {1, 2, 3};
        ASSERT_TRUE(mutable_values.checked_crend().has_value());
        EXPECT_EQ(mutable_values.crend(), *mutable_values.checked_crend());
    }

    TEST_F(method_tests, checked_crend__empty) {
        array<int, 0> values{};
        EXPECT_FALSE(values.checked_crend().has_value());
    }

    TEST_F(method_tests, max_size) {
        static_assert(3 == collections::array<int, 3>::max_size());

        SUCCEED();
    }

    TEST_F(method_tests, empty) {
        static_assert(!collections::array<int, 3>::empty());
        static_assert(!collections::array<int, 1>::empty());
        static_assert(collections::array<int, 0>::empty());

        SUCCEED();
    }

    TEST_F(method_tests, fill__trivial) {
        static_assert([] -> bool {
            array values = {1, 2, 3};
            values.fill(7);
            return 7 == values[0] && 7 == values[1] && 7 == values[2];
        }());

        array values = {1, 2, 3};
        values.fill(7);
        EXPECT_EQ(7, values[0]);
        EXPECT_EQ(7, values[1]);
        EXPECT_EQ(7, values[2]);

        const int* const before = values.data();
        values.fill(0);
        EXPECT_EQ(before, values.data());
        EXPECT_EQ(0, values[0]);
        EXPECT_EQ(0, values[1]);
        EXPECT_EQ(0, values[2]);
    }

    TEST_F(method_tests, fill__nontrivial) {
        static constexpr char cstr[] =
            "a string long enough to defeat the small string optimization";

        array<std::string, 3> values;
        values.fill(cstr);

        EXPECT_EQ(cstr, values[0]);
        EXPECT_EQ(cstr, values[1]);
        EXPECT_EQ(cstr, values[2]);
    }

    TEST_F(method_tests, fill__noexcept_follows_element_copy_assign) {
        static_assert(noexcept(std::declval<array<int, 3>&>().fill(0)));

        static_assert(
            !noexcept(
                std::declval<array<std::string, 2>&>().fill(
                    std::declval<const std::string&>()
                )
            )
        );

        SUCCEED();
    }

    TEST_F(method_tests, fill__single_element) {
        array values = {0};
        values.fill(42);
        EXPECT_EQ(42, values[0]);
    }

    TEST_F(method_tests, swap__trivial) {
        static_assert([] -> bool {
            array first  = {1, 2, 3};
            array second = {4, 5, 6};
            first.swap(second);
            return 4 == first[0] && 6 == first[2] &&
                   1 == second[0] && 3 == second[2];
        }());

        array first  = {1, 2, 3};
        array second = {4, 5, 6};

        const int* const first_data  = first.data();
        const int* const second_data = second.data();

        first.swap(second);

        EXPECT_EQ(4, first[0]);
        EXPECT_EQ(5, first[1]);
        EXPECT_EQ(6, first[2]);
        EXPECT_EQ(1, second[0]);
        EXPECT_EQ(2, second[1]);
        EXPECT_EQ(3, second[2]);
        EXPECT_EQ(first_data,  first.data());
        EXPECT_EQ(second_data, second.data());
    }

    TEST_F(method_tests, swap__nontrivial) {
        static constexpr char long_a[] =
            "a string long enough to defeat the small string optimization";
        static constexpr char long_b[] =
            "another string long enough to defeat the small string "
            "optimization";

        array<std::string, 2> first  = {long_a, "short"};
        array<std::string, 2> second = {long_b, "tiny"};

        first.swap(second);

        EXPECT_EQ(long_b,  first[0]);
        EXPECT_EQ("tiny",  first[1]);
        EXPECT_EQ(long_a,  second[0]);
        EXPECT_EQ("short", second[1]);
    }

    TEST_F(method_tests, swap__noexcept_follows_element_swap) {
        static_assert(
            noexcept(
                std::declval<array<int, 3>&>().swap(
                    std::declval<array<int, 3>&>()
                )
            )
        );

        static_assert(std::is_nothrow_swappable_v<std::string>);

        static_assert(
            noexcept(
                std::declval<array<std::string, 2>&>().swap(
                    std::declval<array<std::string, 2>&>()
                )
            )
        );

        SUCCEED();
    }

    TEST_F(method_tests, sort__no_arg_overload__ascending) {
        array values = {3, 1, 4, 1, 5, 9, 2, 6};
        values.sort();
        EXPECT_TRUE(std::ranges::is_sorted(values));
    }

    TEST_F(method_tests, sort__no_arg_overload__already_sorted) {
        array values = {1, 2, 3, 4, 5};
        values.sort();
        EXPECT_TRUE(std::ranges::is_sorted(values));
    }

    TEST_F(method_tests, sort__no_arg_overload__reverse_sorted) {
        array values = {5, 4, 3, 2, 1};
        values.sort();
        EXPECT_TRUE(std::ranges::is_sorted(values));
    }

    TEST_F(method_tests, sort__no_arg_overload__single_element) {
        array values = {42};
        values.sort();
        EXPECT_EQ(42, values[0]);
    }

    TEST_F(method_tests, sort__no_arg_overload__preserves_all_values) {
        array values = {3, 1, 4, 1, 5, 9, 2, 6};
        const auto before = values;
        values.sort();

        array sorted_before = before;
        sorted_before.sort();
        EXPECT_EQ(sorted_before, values);
    }

    TEST_F(method_tests, sort__range_overload__partial) {
        array values = {5, 3, 1, 4, 2};
        values.sort(values.begin() + 1, values.begin() + 4);

        EXPECT_EQ(5, values[0]);
        EXPECT_TRUE(
            std::is_sorted(values.begin() + 1, values.begin() + 4)
        );
        EXPECT_EQ(2, values[4]);
    }

    TEST_F(method_tests, sort__range_overload__empty_range_no_op) {
        array values = {3, 1, 2};
        const auto before = values;
        values.sort(values.begin(), values.begin());
        EXPECT_EQ(before, values);
    }

    TEST_F(method_tests, sort__range_overload__full_range) {
        array values = {3, 1, 2};
        values.sort(values.begin(), values.end());
        EXPECT_TRUE(std::ranges::is_sorted(values));
    }

    TEST_F(method_tests, sort__predicate_overload__descending) {
        array values = {3, 1, 4, 1, 5, 9, 2, 6};
        values.sort(values.begin(), values.end(), std::greater<int>{});
        EXPECT_TRUE(std::ranges::is_sorted(values, std::greater<int>{}));
    }

    TEST_F(method_tests, sort__predicate_overload__partial_descending) {
        array values = {5, 3, 1, 4, 2};
        values.sort(
            values.begin() + 1,
            values.begin() + 4,
            std::greater<int>{}
        );

        EXPECT_EQ(5, values[0]);
        EXPECT_TRUE(
            std::is_sorted(
                values.begin() + 1,
                values.begin() + 4,
                std::greater<int>{}
            )
        );
        EXPECT_EQ(2, values[4]);
    }

    TEST_F(method_tests, sort__predicate_overload__empty_range_no_op) {
        array values = {3, 1, 2};
        const auto before = values;
        values.sort(values.begin(), values.begin(), std::less<int>{});
        EXPECT_EQ(before, values);
    }

    TEST_F(method_tests, sort__nontrivial) {
        array<std::string, 4> values = {
            "banana", "apple", "date", "cherry"
        };

        values.sort();
        EXPECT_TRUE(std::ranges::is_sorted(values));
    }

    TEST_F(method_tests, stable_sort__no_arg_overload__ascending) {
        array values = {3, 1, 4, 1, 5, 9, 2, 6};
        values.stable_sort();
        EXPECT_TRUE(std::ranges::is_sorted(values));
    }

    TEST_F(
        method_tests,
        stable_sort__predicate_overload__preserves_relative_order
    ) {
        struct item {
            int key;

            int order;

            auto operator==(const struct item&) const -> bool = default;

            auto operator<=>(const struct item&) const = default;
        };

        array<struct item, 6> values = {
            item{.key=2, .order=0},
            item{.key=1, .order=1},
            item{.key=2, .order=2},
            item{.key=1, .order=3},
            item{.key=2, .order=4},
            item{.key=1, .order=5}
        };

        values.stable_sort(
            values.begin(),
            values.end(),
            [](const struct item& a, const struct item& b) -> bool {
                return a.key < b.key;
            }
        );

        ASSERT_EQ(1, values[0].key);
        ASSERT_EQ(1, values[1].key);
        ASSERT_EQ(1, values[2].key);
        ASSERT_EQ(2, values[3].key);
        ASSERT_EQ(2, values[4].key);
        ASSERT_EQ(2, values[5].key);

        EXPECT_EQ(1, values[0].order);
        EXPECT_EQ(3, values[1].order);
        EXPECT_EQ(5, values[2].order);
        EXPECT_EQ(0, values[3].order);
        EXPECT_EQ(2, values[4].order);
        EXPECT_EQ(4, values[5].order);
    }

    TEST_F(method_tests, stable_sort__range_overload__partial) {
        array values = {5, 3, 1, 4, 2};
        values.stable_sort(values.begin() + 1, values.begin() + 4);

        EXPECT_EQ(5, values[0]);
        EXPECT_TRUE(
            std::is_sorted(values.begin() + 1, values.begin() + 4)
        );
        EXPECT_EQ(2, values[4]);
    }

    TEST_F(method_tests, stable_sort__range_overload__empty_range_no_op) {
        array values = {3, 1, 2};
        const auto before = values;
        values.stable_sort(values.begin(), values.begin());
        EXPECT_EQ(before, values);
    }

    TEST_F(method_tests, stable_sort__range_overload__full_range) {
        array values = {3, 1, 2};
        values.stable_sort(values.begin(), values.end());
        EXPECT_TRUE(std::ranges::is_sorted(values));
    }

    TEST_F(method_tests, stable_sort__predicate_overload__descending) {
        array values = {3, 1, 4, 1, 5, 9, 2, 6};
        values.stable_sort(
            values.begin(),
            values.end(),
            std::greater<int>{}
        );

        EXPECT_TRUE(std::ranges::is_sorted(values, std::greater<int>{}));
    }

    TEST_F(method_tests, stable_sort__predicate_overload__empty_range_no_op) {
        array values = {3, 1, 2};
        const auto before = values;
        values.stable_sort(
            values.begin(),
            values.begin(),
            std::less<int>{}
        );
        EXPECT_EQ(before, values);
    }

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
        static_assert(sizeof(array<double, 3>) == sizeof(double[3]));
        static_assert(alignof(array<double, 3>) == alignof(double[3]));

        static_assert(std::is_standard_layout_v<array<std::string, 3>>);
        static_assert(
            sizeof(array<std::string, 3>) == sizeof(std::string[3])
        );
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