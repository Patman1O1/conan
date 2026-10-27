// ISO C++ Includes
#include <algorithm>
#include <concepts>
#include <cstddef>
#include <expected>
#include <functional>
#include <random>
#include <ranges>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

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
        concept deducible = requires {
            array{std::declval<Args>()...};
        };

        // ── Test Fixtures ───────────────────────────────────────────────────
        class operator_tests : public testing::Test {};

        class method_tests : public testing::Test {};

        class traits_tests : public testing::Test {};
    } // namespace

    // ── Operator Tests ──────────────────────────────────────────────────────
    // ── Method Tests ────────────────────────────────────────────────────────
    // ── Trait Tests ─────────────────────────────────────────────────────────

    
} // namespace collections::array_testing
