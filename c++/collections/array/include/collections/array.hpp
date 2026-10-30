#ifndef COLLECTIONS_ARRAY_HPP
#define COLLECTIONS_ARRAY_HPP

// ISO C Includes
#include <cstddef>

// ISO C++ Includes
#include <algorithm>
#include <compare>
#include <concepts>
#include <expected>
#include <functional>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <type_traits>

namespace collections {
    template<typename T, std::size_t N>
    class array {
    public:
        // ── Aliases ─────────────────────────────────────────────────────────
        using value_type = T;

        using size_type = std::size_t;

        using difference_type = std::ptrdiff_t;

        using pointer = value_type*;

        using const_pointer = const value_type*;

        using reference = value_type&;

        using const_reference = const value_type&;

        using iterator = pointer;

        using const_iterator = const_pointer;

        using reverse_iterator = std::reverse_iterator<iterator>;

        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        // ── Fields ──────────────────────────────────────────────────────────
        value_type values[N];

        // ── Overloaded Operators ────────────────────────────────────────────
        [[nodiscard]]
        constexpr auto operator==(const array& rhs) const noexcept(
            noexcept(std::declval<value_type>() == std::declval<value_type>())
        ) -> bool {
            return std::equal(this->begin(), this->end(), rhs.begin());
        }

        [[nodiscard]]
        constexpr auto operator<=>(const array& rhs) const noexcept(
            noexcept(std::declval<value_type>() <=> std::declval<value_type>())
        ) -> std::compare_three_way_result_t<value_type> {
            return std::lexicographical_compare_three_way(
                this->values, this->values + N,
                rhs.values, rhs.values + N
            );
        }
        
        [[nodiscard]]
        constexpr auto operator[](
            const size_type index
        ) noexcept -> reference { return this->values[index]; }

        [[nodiscard]]
        constexpr auto operator[](
            const size_type index
        ) const noexcept -> const_reference { return this->values[index]; }

    private:
        // ── Methods ─────────────────────────────────────────────────────────
        template<
            std::contiguous_iterator Iterator = iterator,
            std::predicate<value_type, value_type> Predicate
        >
        constexpr void _sort(
            Iterator first,
            Iterator last,
            Predicate pred
        ) { std::sort(first, last, pred); }

        template<
            std::contiguous_iterator Iterator = iterator,
            std::predicate<value_type, value_type> Predicate
        >
        constexpr void _stable_sort(
            Iterator first,
            Iterator last,
            Predicate pred
        ) { std::stable_sort(first, last, pred); }

    public:
        // ── Methods ─────────────────────────────────────────────────────────
        [[nodiscard]]
        constexpr auto at(const size_type index) -> reference {
            if (index >= N) [[unlikely]] {
                throw std::out_of_range(
                    "collections::array::at index out of range"
                );
            }
            return this->values[index];
        }

        [[nodiscard]]
        constexpr auto at(const size_type index) const -> const_reference {
            if (index >= N) [[unlikely]] {
                throw std::out_of_range(
                    "collections::array::at index out of range"
                );
            }
            return this->values[index];
        }

        [[nodiscard]]
        constexpr auto expect_at(
            const size_type index
        ) noexcept -> std::expected<
            std::reference_wrapper<value_type>,
            std::error_code
        > {
            if (index >= N) [[unlikely]] {
                return std::unexpected(
                    std::make_error_code(std::errc::result_out_of_range)
                );
            }
            return std::reference_wrapper<value_type>(this->values[index]);
        }

        [[nodiscard]]
        constexpr auto expect_at(
            const size_type index
        ) const noexcept -> std::expected<
            std::reference_wrapper<const value_type>,
            std::error_code
        > {
            if (index >= N) [[unlikely]] {
                return std::unexpected(
                    std::make_error_code(std::errc::result_out_of_range)
                );
            }

            return std::reference_wrapper<const value_type>(
                this->values[index]
            );
        }

        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto front() noexcept -> reference {
            return this->values[0];
        }

        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto front() const noexcept -> const_reference {
            return this->values[0];
        }

        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto back() noexcept -> reference {
            return this->values[array::size() - 1];
        }

        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto back() const noexcept -> const_reference {
            return this->values[array::size() - 1];
        }

        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto data() noexcept -> pointer { return this->values; }

        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto data() const noexcept -> const_pointer {
            return this->values;
        }

        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto begin() noexcept -> iterator { return this->values; }

        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto end() noexcept -> iterator { return this->values + N; }

        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto begin() const noexcept -> const_iterator {
            return this->values;
        }

        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto end() const noexcept -> const_iterator {
            return this->values + N;
        }

        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto cbegin() const noexcept -> const_iterator {
            return this->values;
        }

        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto cend() const noexcept -> const_iterator {
            return this->values + N;
        }

        [[nodiscard]]
        constexpr auto rbegin() noexcept -> reverse_iterator {
            return reverse_iterator(this->end());
        }

        [[nodiscard]]
        constexpr auto rend() noexcept -> reverse_iterator {
            return reverse_iterator(this->begin());
        }

        [[nodiscard]]
        constexpr auto rbegin() const noexcept -> const_reverse_iterator {
            return const_reverse_iterator(this->end());
        }

        [[nodiscard]]
        constexpr auto rend() const noexcept -> const_reverse_iterator {
            return const_reverse_iterator(this->begin());
        }

        [[nodiscard]]
        constexpr auto crbegin() const noexcept -> const_reverse_iterator {
            return this->rbegin();
        }

        [[nodiscard]]
        constexpr auto crend() const noexcept -> const_reverse_iterator {
            return this->rend();
        }

        [[nodiscard]]
        constexpr auto checked_begin() noexcept -> std::optional<iterator> {
            return N > 0 ? this->values : std::nullopt;
        }

        [[nodiscard]]
        constexpr auto checked_end() noexcept -> std::optional<iterator> {
            return N > 0 ? this->values + N : std::nullopt;
        }

        [[nodiscard]]
        constexpr auto checked_begin() const noexcept
            -> std::optional<const_iterator> {
            return N > 0 ? this->values : std::nullopt;
        }

        [[nodiscard]]
        constexpr auto checked_end() const noexcept
            -> std::optional<const_iterator> {
            return N > 0 ? this->values + N : std::nullopt;
        }

        [[nodiscard]]
        constexpr auto checked_cbegin() const noexcept
            -> std::optional<const_iterator> {
            return N > 0 ? this->values : std::nullopt;
        }

        [[nodiscard]]
        constexpr auto checked_cend() const noexcept
            -> std::optional<const_iterator> {
            return N > 0 ? this->values + N : std::nullopt;
        }

        [[nodiscard]]
        constexpr auto checked_rbegin() noexcept
            -> std::optional<reverse_iterator> {
            return N > 0 ? reverse_iterator(this->end()) : std::nullopt;
        }

        [[nodiscard]]
        constexpr auto checked_rend() noexcept
            -> std::optional<reverse_iterator> {
            return N > 0 ? reverse_iterator(this->begin()) : std::nullopt;
        }

        [[nodiscard]]
        constexpr auto checked_rbegin() const noexcept
            -> std::optional<const_reverse_iterator> {
            return N > 0 ? const_reverse_iterator(this->end()) : std::nullopt;
        }

        [[nodiscard]]
        constexpr auto checked_rend() const noexcept
            -> std::optional<const_reverse_iterator> {
            return N > 0
                ? const_reverse_iterator(this->begin())
                : std::nullopt;
        }

        [[nodiscard]]
        constexpr auto checked_crbegin() const noexcept
            -> std::optional<const_reverse_iterator> {
            return N > 0 ? this->rbegin() : std::nullopt;
        }

        [[nodiscard]]
        constexpr auto checked_crend() const noexcept
            -> std::optional<const_reverse_iterator> {
            return N > 0 ? this->rend() : std::nullopt;
        }

        [[nodiscard]]
        [[gnu::always_inline]]
        static constexpr auto empty() noexcept -> bool { return N == 0; }

        [[nodiscard]]
        [[gnu::always_inline]]
        static constexpr auto size() noexcept -> size_type { return N; }

        [[nodiscard]]
        [[gnu::always_inline]]
        static constexpr auto max_size() noexcept -> size_type { return N; }

        constexpr void fill(
            const_reference value
        ) noexcept (
            std::is_nothrow_copy_assignable_v<value_type>
        ) { std::fill(this->begin(), this->end(), value); }

        constexpr void swap(
            array& other
        ) noexcept (
            noexcept(std::is_nothrow_swappable_v<value_type>)
        ) { std::swap(this->values, other.values); }

        constexpr void sort() {
            this->_sort(this->begin(), this->end(), std::less<value_type>{});
        }

        template<std::contiguous_iterator Iterator = iterator>
        constexpr void sort(Iterator first, Iterator last) {
            if (first == last) [[unlikely]] {
                return;
            }
            this->_sort(first, last, std::less<value_type>{});
        }

        template<
            std::contiguous_iterator Iterator = iterator,
            std::predicate<value_type, value_type> Predicate
        >
        constexpr void sort(Iterator first, Iterator last, Predicate pred) {
            if (first == last) [[unlikely]] {
                return;
            }
            this->_sort(first, last, pred);
        }

        constexpr void stable_sort() {
            this->_stable_sort(
                this->begin(),
                this->end(),
                std::less<value_type>{}
            );
        }

        template<std::contiguous_iterator Iterator = iterator>
        constexpr void stable_sort(Iterator first, Iterator last) {
            if (first == last) [[unlikely]] {
                return;
            }
            this->_stable_sort(first, last, std::less<value_type>{});
        }

        template<
            std::contiguous_iterator Iterator,
            std::predicate<value_type, value_type> Predicate
        >
        constexpr void stable_sort(
            Iterator first,
            Iterator last,
            Predicate pred
        ) {
            if (first == last) [[unlikely]] {
                return;
            }
            this->_stable_sort(first, last, pred);
        }
    };

    // ── Deduction Guides ────────────────────────────────────────────────────
    template<typename T, typename... U> requires (std::same_as<T, U> && ...)
    array(T, U...) -> array<T, 1 + sizeof...(U)>;
} // namespace collections
#endif // #ifndef COLLECTIONS_ARRAY_HPP
