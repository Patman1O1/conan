#ifndef COLLECTIONS_CIRCULAR_INPLACE_VECTOR_HPP
#define COLLECTIONS_CIRCULAR_INPLACE_VECTOR_HPP

// ISO C Includes
#include <cstddef>
#include <cstdint>

// ISO C++ Includes
#include <algorithm>
#include <compare>
#include <concepts>
#include <iterator>
#include <type_traits>

namespace collections {
    template<typename T, std::size_t N>
    class circular_inplace_vector {
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

    private:
        // ── Fields ──────────────────────────────────────────────────────────
        value_type values_[N];

        size_type sz_;

    public:
        // ── Constructors ────────────────────────────────────────────────────
        // TODO: Need to implement
        constexpr circular_inplace_vector() noexcept;

        // TODO: Need to implement
        constexpr explicit circular_inplace_vector(const size_type count);

        // TODO: Need to implement
        constexpr circular_inplace_vector(
            const size_type count,
            const_reference value
        );

        // TODO: Need to implement
        template<std::input_iterator InputIt>
        constexpr circular_inplace_vector(InputIt first, InputIt last);

        // TODO: Need to implement
        template<std::ranges::input_range R> requires (
            std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        )
        constexpr circular_inplace_vector(std::from_range_t, R&& rg);

        // TODO: Need to implement
        constexpr circular_inplace_vector(
            const circular_inplace_vector& other
        );

        // TODO: Need to implement
        constexpr circular_inplace_vector(
            circular_inplace_vector&& other
        ) noexcept(
            N == 0 || std::is_nothrow_move_constructible_v<value_type>
        );

        // TODO: Need to implement
        constexpr circular_inplace_vector(
            std::initializer_list<value_type> values
        );
        
        // ── Destructor ──────────────────────────────────────────────────────
        constexpr ~circular_inplace_vector() noexcept = default;

        // ── Overloaded Operators ────────────────────────────────────────────
        // TODO: Need to implement
        constexpr auto operator=(
            const circular_inplace_vector& other
        ) -> circular_inplace_vector&;

        // TODO: Need to implement
        constexpr auto operator=(circular_inplace_vector&& other) noexcept(
            N == 0 ||
            (std::is_nothrow_move_assignable_v<value_type> &&
            std::is_nothrow_move_constructible_v<value_type>)
        ) -> circular_inplace_vector&;

        // TODO: Need to implement
        constexpr auto operator=(
            std::initializer_list<value_type> values
        ) -> circular_inplace_vector&;

        [[nodiscard]]
        constexpr auto operator==(
            const circular_inplace_vector& rhs
        ) const noexcept(
            noexcept(std::declval<value_type>() == std::declval<value_type>())
        ) -> bool {
            return std::equal(this->begin(), this->end(), rhs.begin());
        }

        [[nodiscard]]
        constexpr auto operator<=>(
            const circular_inplace_vector& rhs
        ) const noexcept(
            noexcept(std::declval<value_type>() <=> std::declval<value_type>())
        ) -> std::compare_three_way_result_t<value_type> {
            return std::lexicographical_compare_three_way(
                this->values_, this->values_ + N,
                rhs.values_, rhs.values_ + N
            );
        }
        
        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto operator[](
            const size_type index
        ) noexcept -> reference;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto operator[](
            const size_type index
        ) const noexcept -> const_reference;

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
        // TODO: Need to implement
        constexpr void assign(const size_type count, const_reference value);

        // TODO: Need to implement
        template<std::input_iterator InputIt>
        constexpr void assign(InputIt first, InputIt last);

        // TODO: Need to implement
        constexpr void assign(std::initializer_list<T> ilist);

        // TODO: Need to implement
        template<std::ranges::input_range R> requires(
            std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        )
        constexpr void assign_range(R&& rg);
        
        // TODO: Need to implement
        constexpr auto at(const size_type index) -> reference;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto at(const size_type index) const -> const_reference;

        // TODO: Need to implement
        constexpr auto front() -> reference;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto front() const -> const_reference;

        // TODO: Need to implement
        constexpr auto back() -> reference;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto back() const -> const_reference;

        // TODO: Need to implement
        constexpr auto data() noexcept -> pointer;

        // TODO: Need to implement
        constexpr auto data() const noexcept -> const_pointer;

        // TODO: Need to implement
        constexpr auto begin() noexcept -> iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto begin() const noexcept -> const_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto cbegin() const noexcept -> const_iterator;

        // TODO: Need to implement
        constexpr auto rbegin() noexcept -> reverse_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto rbegin() const noexcept -> const_reverse_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto crbegin() const noexcept -> const_reverse_iterator;

        // TODO: Need to implement
        constexpr auto end() noexcept -> iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto end() const noexcept -> const_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto cend() const noexcept -> const_iterator;

        // TODO: Need to implement
        constexpr auto rend() noexcept -> reverse_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto rend() const noexcept -> const_reverse_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto crend() const noexcept -> const_reverse_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto empty() const noexcept -> bool;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto size() const -> size_type;

        // TODO: Need to implement
        static constexpr auto max_size() noexcept -> size_type;

        // TODO: Need to implement
        static constexpr auto capacity() noexcept -> size_type;

        // TODO: Need to implement
        constexpr void resize(const size_type count);

        // TODO: Need to implement
        static constexpr void reserve(const size_type new_cap);

        // TODO: Need to implement
        static constexpr void shrink_to_fit() noexcept;

        // TODO: Need to implement
        constexpr auto insert(
            const_iterator index,
            const_reference value
        ) -> iterator;

        // TODO: Need to implement
        constexpr auto insert(
            const_iterator index,
            value_type&& value
        ) -> iterator;

        // TODO: Need to implement
        constexpr auto insert(
            const_iterator index,
            const size_type count,
            const_reference value
        ) -> iterator;

        // TODO: Need to implement
        template<std::input_iterator InputIt>
        constexpr auto insert(
            const_iterator index,
            InputIt first,
            InputIt last
        ) -> iterator;

        // TODO: Need to implement
        constexpr auto insert(
            const_iterator index,
            std::initializer_list<value_type> ilist
        ) -> iterator;

        // TODO: Need to implement
        template<std::ranges::input_range R> requires(
            std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        )
        constexpr auto insert_range(
            const_iterator index,
            R&& rg
        ) -> iterator;

        // TODO: Need to implement
        template<typename... Args>
        constexpr auto emplace(
            const_iterator index,
            Args&&... args
        ) -> iterator;

        // TODO: Need to implement
        template<typename... Args>
        constexpr auto emplace_back(Args&&... args) -> reference;

        // TODO: Need to implement
        template<typename... Args>
        constexpr auto try_emplace_back(
            Args&&... args
        ) -> std::optional<reference>;

        // TODO: Need to implement
        template<typename... Args>
        constexpr auto unchecked_emplace_back(Args&&... args) -> reference;

        // TODO: Need to implement
        constexpr auto push_back(const_reference value) -> reference;

        // TODO: Need to implement
        constexpr auto push_back(value_type&& value) -> reference;

        // TODO: Need to implement
        constexpr auto try_push_back(
            const_reference value
        ) -> std::optional<reference>;

        // TODO: Need to implement
        constexpr auto try_push_back(
            value_type&& value
        ) -> std::optional<reference>;

        // TODO: Need to implement
        constexpr auto unchecked_push_back(
            const_reference value
        ) -> reference;

        // TODO: Need to implement
        constexpr auto unchecked_push_back(
            value_type&& value
        ) -> reference;

        // TODO: Need to implement
        constexpr void pop_back();

        // TODO: Need to implement
        template<std::ranges::input_range R> requires(
            std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        )
        constexpr void append_range(R&& rg);

        // TODO: Need to implement
        constexpr void clear() noexcept;

        // TODO: Need to implement
		constexpr auto erase(const_iterator pos) -> iterator;

        // TODO: Need to implement
        constexpr auto erase(
            const_iterator first,
            const_iterator last
        ) -> iterator;

        // TODO: Need to implement
        constexpr void swap(circular_inplace_vector& other) noexcept(
            N == 0 ||
            (std::is_nothrow_swappable_v<value_type> &&
             std::is_nothrow_move_constructible_v<value_type>)
        );
    };

    // ── Deduction Guides ────────────────────────────────────────────────────

    // ── Functions ───────────────────────────────────────────────────────────
    template<typename T, std::size_t N>
    constexpr void swap(
        circular_inplace_vector<T, N>& lhs,
        circular_inplace_vector<T, N>& rhs
    ) noexcept(
        N == 0 ||
        (std::is_nothrow_swappable_v<T> && 
        std::is_nothrow_move_constructible_v<T>)
    );

    template<typename T, std::size_t N, typename U = T>
    constexpr auto erase(
        circular_inplace_vector<T, N>& vec,
        const U& value
    ) -> typename circular_inplace_vector<T, N>::size_type;

    template<typename T, std::size_t N, typename Pred>
    constexpr auto erase_if(
        circular_inplace_vector<T, N>& vec,
        Pred pred
    ) -> typename circular_inplace_vector<T, N>::size_type;
} // namespace collections
#endif // #ifndef COLLECTIONS_CIRCULAR_INPLACE_VECTOR_HPP
