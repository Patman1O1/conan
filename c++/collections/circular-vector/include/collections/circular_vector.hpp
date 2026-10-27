#ifndef COLLECTIONS_CIRCULAR_VECTOR_HPP
#define COLLECTIONS_CIRCULAR_VECTOR_HPP

// ISO C Includes
#include <cstddef>
#include <cstdint>

// ISO C++ Includes
#include <algorithm>
#include <bit>
#include <compare>
#include <concepts>
#include <expected>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <type_traits>

namespace collections {
    template<typename T, typename Alloc = std::allocator<T>>
    class circular_vector {
    public:
        // ── Aliases ─────────────────────────────────────────────────────────
        using value_type = T;

        using size_type = std::size_t;

        using difference_type = std::ptrdiff_t;

        using allocator_type = Alloc;

        using allocator_traits = std::allocator_traits<allocator_type>;

        using pointer = typename allocator_traits::pointer;

        using const_pointer = typename allocator_traits::const_pointer;

        using reference = value_type&;

        using const_reference = const value_type&;

        using iterator = pointer;

        using const_iterator = const_pointer;

        using reverse_iterator = std::reverse_iterator<iterator>;

        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        // ── error ───────────────────────────────────────────────────────────
        enum class error : std::uint8_t { out_of_range };

    private: 
        // ── Fields ──────────────────────────────────────────────────────────
        static constexpr size_type DEFAULT_CAP = 16;

        pointer vals_;

        size_type sz_, cap_;
        
        [[no_unique_address]]
        allocator_type alloc_;

        // ── Methods ─────────────────────────────────────────────────────────
        [[gnu::always_inline]]
        constexpr void _alloc(pointer& ptr, size_type& count) {
            auto result = allocator_traits::allocate_at_least(
                this->alloc_,
                count
            );

            ptr = result.ptr;
            count = result.count;
        }

        [[gnu::always_inline]]
        constexpr void _alloc() {
            auto result = allocator_traits::allocate_at_least(
                this->alloc_,
                this->cap_
            );

            this->vals_ = result.ptr;
            this->cap_ = result.count;
        }

        [[gnu::always_inline]]
        constexpr void _dealloc(pointer ptr, const size_type count) noexcept {
            allocator_traits::deallocate(this->alloc_, ptr, count);
        }

        [[gnu::always_inline]]
        constexpr void _dealloc() noexcept {
            allocator_traits::deallocate(
                this->alloc_,
                this->vals_,
                this->cap_
            );
        }

        [[gnu::always_inline]]
        constexpr void _dealloc_zeroed(
            pointer& ptr,
            size_type& count
        ) noexcept {
            if (ptr == nullptr) [[unlikely]] {
                return;
            }

            allocator_traits::deallocate(this->alloc_, ptr, count);
            ptr = nullptr;
            count = 0;
        }

        [[gnu::always_inline]]
        constexpr void _dealloc_zeroed() noexcept {
            allocator_traits::deallocate(
                this->alloc_,
                this->vals_,
                this->cap_
            );
            this->vals_ = nullptr;
            this->cap_ = 0;
        }

        [[gnu::always_inline]]
        constexpr void _destroy() noexcept {
            std::destroy_n(this->vals_, this->cap_);
        }

        template <std::ranges::input_range R>
        requires std::convertible_to<
            std::ranges::range_reference_t<R>,
            value_type
        >
        constexpr void _construct(std::from_range_t, R&& rg) {
            auto construct_impl = [this](auto&& range) {
                pointer dest_begin = this->vals_ + this->sz_;
                pointer dest_end = this->vals_ + this->cap_;

                auto result = std::ranges::uninitialized_copy(
                    std::forward<decltype(range)>(range),
                    dest_begin,
                    dest_end
                );

                this->sz_ += static_cast<size_type>(result.out - dest_begin);
            };

            if constexpr (std::is_rvalue_reference_v<R&&>) {
                construct_impl(std::forward<R>(rg) | std::views::as_rvalue);
            } else {
                construct_impl(rg);
            }
        }

        constexpr void _realloc(size_type new_cap) {
            pointer new_ptr;
            this->_alloc(new_ptr, new_cap);

            try {
                if constexpr (
                    std::is_nothrow_move_constructible_v<value_type> || 
                    !std::is_copy_constructible_v<value_type>
                ) {
                    std::ranges::uninitialized_move_n(
                        this->vals_,
                        this->sz_,
                        new_ptr
                    );
                } else {
                    std::ranges::uninitialized_copy_n(
                        this->vals_,
                        this->sz_,
                        new_ptr
                    );
                }
            } catch (...) {
                this->_dealloc(new_ptr, new_cap);
                throw;
            }

            this->_destroy();
            this->_dealloc();

            this->vals_ = new_ptr;
            this->cap_ = new_cap;
        }
               
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
        // ── Constructors ────────────────────────────────────────────────────
        // TODO: Need to test
        constexpr circular_vector() noexcept(
            noexcept(allocator_type())
        ) : vals_(nullptr), sz_(0), cap_(DEFAULT_CAP) {}

        // TODO: Need to test
        explicit constexpr circular_vector(
            const allocator_type& alloc
        ) : vals_(nullptr), sz_(0), cap_(DEFAULT_CAP), alloc_(alloc) {}

        // TODO: Need to test
        explicit constexpr circular_vector(
            const size_type count,
            const allocator_type& alloc = allocator_type()
        ) : alloc_(alloc), sz_(count) {
            // Make the capacity the next power of 2 up with respect to `count`
            // if `count` exceeds the default capacity
            if (count > DEFAULT_CAP) {
                this->cap_ = std::bit_ceil(count);
            }
            
            this->_alloc();
            this->_construct(
                std::from_range,
                std::views::repeat(value_type(), count)
            );
        }

        // TODO: Need to test
        constexpr circular_vector(
            const size_type count,
            const_reference value,
            const allocator_type& alloc = allocator_type()
        ) : alloc_(alloc), sz_(count) {
            // Make the capacity the next power of 2 up with respect to `count`
            // if `count` exceeds the default capacity
            if (count > DEFAULT_CAP) {
                this->cap_ = std::bit_ceil(count);
            }

            this->_alloc();
            this->_construct(
                std::from_range,
                std::views::repeat(value, count)
            );
        }

        // TODO: Need to test
        template<std::input_iterator InputIt>
        constexpr circular_vector(
            InputIt first,
            InputIt last,
            const allocator_type& alloc
        ) : alloc_(alloc), cap_(std::distance(first, last)) {
            this->_alloc();
            this->_construct(
                std::from_range,
                std::ranges::subrange(first, last)
            );
        }

        // TODO: Need to test
        template<std::ranges::input_range R> requires(
            std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        ) constexpr circular_vector(
            std::from_range_t,
            R&& rg,
            const allocator_type& alloc = allocator_type()
        ) : alloc_(alloc), sz_(0) {
            if constexpr (std::ranges::sized_range<R>) {
                if (std::ranges::size(rg) == 0) [[unlikely]] {
                    this->cap_ = DEFAULT_CAP;
                    this->_alloc();
                    return;
                }

                // Set the capacity to the nearest power of 2
                // that is no less than the range size
                this->cap_ = std::bit_ceil(std::ranges::size(rg));

                this->_alloc();
                this->_construct(std::from_range, rg);
            } else {
                this->cap_ = DEFAULT_CAP;

                this->_alloc();
                for (auto&& val : std::forward<R>(rg)) {
                    if (this->sz_ == this->cap_) {
                        this->_realloc(this->cap_ << 1);
                    }

                    std::construct_at(
                        this->vals_ + (this->sz_++),
                        std::forward<decltype(val)>(val)
                    );
                }
            }
        }

        // TODO: Need to test
        constexpr circular_vector(
            const circular_vector& other
        ) : cap_(other.cap_), sz_(other.sz_) {
            this->_alloc();
            this->_construct(
                std::from_range,
                other
            );
        }

        // TODO: Need to test
        constexpr circular_vector(
            circular_vector&& other
        ) noexcept : cap_(other.cap_), sz_(other.sz_), vals_(other.vals_) {
            other.vals_ = nullptr;
            other.sz_ = 0;
        }

        // TODO: Need to test
        constexpr circular_vector(
            const circular_vector& other,
            const std::type_identity_t<allocator_type>& alloc
        ) : alloc_(alloc), cap_(other.cap_), sz_(other.sz_) {
            this->_alloc();
            this->_construct(
                std::from_range,
                other
            );
        }

         // TODO: Need to test
        constexpr circular_vector(
            circular_vector&& other,
            const std::type_identity_t<allocator_type>& alloc
        ) noexcept : alloc_(alloc),
                     cap_(other.cap_),
                     sz_(other.sz_),
                     vals_(other.vals_) {
            other.vals_ = nullptr;
            other.sz_ = 0;
        }

        // TODO: Need to test
        constexpr circular_vector(
            std::initializer_list<value_type> values,
            const allocator_type& alloc = allocator_type()
        ) : alloc_(alloc), sz_(std::distance(values.begin(), values.end())) {
            this->cap_ = std::bit_ceil(this->sz_);
            this->_alloc();
            this->_construct(
                std::from_range, 
                std::ranges::subrange(values.begin(), values.end())
            );
        }

        // ── Destructor ──────────────────────────────────────────────────────
        // TODO: Need to test
        constexpr ~circular_vector() noexcept {
            if (this->vals_ == nullptr) [[unlikely]] {
                return;
            }

            this->_destroy();
            this->_dealloc_zeroed();
        }

        // ── Overloaded Operators ────────────────────────────────────────────
        // TODO: Need to test
        constexpr auto operator=(
            const circular_vector& rhs
        ) -> circular_vector& {
            // Protect against self-assignment
            if (this == &rhs) [[unlikely]] {
                return *this;
            }

            if (this->sz_ > 0) [[likely]] {
                this->_destroy();
                this->_dealloc();
            }
            
            this->cap_ = rhs.cap_;
            this->_alloc();
            this->_construct(std::from_range, rhs);

            return *this;
        }

        // TODO: Need to test
        constexpr auto operator=(
            circular_vector&& rhs
        ) noexcept -> circular_vector& {
            if (this == &rhs) [[unlikely]] {
                return *this;
            }

            if (this->sz_ > 0) [[likely]] {
                this->_destroy();
                this->_dealloc();
            }

            this->vals_ = rhs.vals_;
            rhs.vals_ = nullptr;

            this->cap_ = rhs.cap_;
            
            this->sz_ = rhs.sz_;
            rhs.sz_ = 0;

            return *this;
        }

        // TODO: Need to test
        constexpr auto operator=(
            std::initializer_list<value_type> values
        ) -> circular_vector& {
            if (this->sz_ > 0) [[likely]] {
                this->_destroy();
                this->_dealloc();
            }

            this->_alloc();
            this->_construct(
                std::from_range,
                std::ranges::subrange(values.begin(), values.end())
            );
        }

        // TODO: Need to test
        [[nodiscard]]
        constexpr auto operator==(const circular_vector& rhs) const noexcept(
            noexcept(std::declval<value_type>() == std::declval<value_type>())
        ) -> bool {
            return std::equal(this->begin(), this->end(), rhs.begin());
        }

        // TODO: Need to test
        [[nodiscard]]
        constexpr auto operator<=>(const circular_vector& rhs) const noexcept(
            noexcept(std::declval<value_type>() <=> std::declval<value_type>())
        ) -> std::compare_three_way_result_t<value_type> {
            return std::lexicographical_compare_three_way(
                this->vals_, this->vals_ + this->sz_,
                rhs.vals_, rhs.vals_ + this->sz_
            );
        }
       
        // TODO: Need to test
        [[nodiscard]]
        constexpr auto operator[](
            const size_type index
        ) noexcept -> reference { return this->vals_[index]; }

        // TODO: Need to test
        [[nodiscard]]
        constexpr auto operator[](
            const size_type index
        ) const noexcept -> const_reference { return this->vals_[index]; }

        // ── Methods ────────────────────────────────────────────────────────
        // TODO: Need to implement
        constexpr void assign(const size_type count, const_reference value) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        template<std::input_iterator InputIt>
        constexpr void assign(InputIt first, InputIt last) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr void assign(std::initializer_list<value_type> values) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        template<std::ranges::input_range R> requires(
            std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        )
        constexpr void assign_range(R&& rg) {

        }

        // TODO: Need to test
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr allocator_type get_allocator() const noexcept {
            return this->alloc_;
        }

        // TODO: Need to test
        [[nodiscard]]
        constexpr auto at(const size_type index) -> reference {
            if (index >= this->sz_) [[unlikely]] {
                throw std::out_of_range(
                    "collections::circular_vector::at index out of range"
                );
            }
            return this->vals_[index];
        }

        // TODO: Need to test
        [[nodiscard]]
        constexpr auto at(const size_type index) const -> const_reference {
            if (index >= this->sz_) [[unlikely]] {
                throw std::out_of_range(
                    "collections::circular_vector::at index out of range"
                );
            }
            return this->vals_[index];
        }

        // TODO: Need to test
        [[nodiscard]]
        constexpr auto expect_at(
            const size_type index
        ) noexcept -> std::expected<
            std::reference_wrapper<value_type>, error
        > {
            if (index >= this->sz_) [[unlikely]] {
                return std::unexpected(error::out_of_range);
            }
            return std::reference_wrapper<value_type>(this->vals_[index]);
        }

        // TODO: Need to test
        [[nodiscard]]
        constexpr auto expect_at(
            const size_type index
        ) const noexcept -> std::expected<
            std::reference_wrapper<const value_type>, error
        > {
            if (index >= this->sz_) [[unlikely]] {
                return std::unexpected(error::out_of_range);
            }

            return std::reference_wrapper<const value_type>(
                this->vals_[index]
            );
        }

        // TODO: Need to test
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto front() noexcept -> reference {
            return this->vals_[0];
        }

        // TODO: Need to test
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto front() const noexcept -> const_reference {
            return this->vals_[0];
        }

        // TODO: Need to test
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto back() noexcept -> reference {
            return this->vals_[this->size() - 1];
        }

        // TODO: Need to test
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto back() const noexcept -> const_reference {
            return this->vals_[this->sz_ - 1];
        }

        // TODO: Need to test
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto data() noexcept -> pointer { return this->vals_; }

        // TODO: Need to test
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto data() const noexcept -> const_pointer {
            return this->vals_;
        }

        // TODO: Need to test
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto begin() noexcept -> iterator { return this->vals_; }

        // TODO: Need to test
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto end() noexcept -> iterator {
            return this->vals_ + this->sz_;
        }

        // TODO: Need to test
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto begin() const noexcept -> const_iterator {
            return this->vals_;
        }

        // TODO: Need to test
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto end() const noexcept -> const_iterator {
            return this->vals_ + this->sz_;
        }

        // TODO: Need to test
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto cbegin() const noexcept -> const_iterator {
            return this->vals_;
        }

        // TODO: Need to test
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto cend() const noexcept -> const_iterator {
            return this->vals_ + this->sz_;
        }

        // TODO: Need to test
        [[nodiscard]]
        constexpr auto rbegin() noexcept -> reverse_iterator {
            return reverse_iterator(this->end());
        }

        // TODO: Need to test
        [[nodiscard]]
        constexpr auto rend() noexcept -> reverse_iterator {
            return reverse_iterator(this->begin());
        }

        // TODO: Need to test
        [[nodiscard]]
        constexpr auto rbegin() const noexcept -> const_reverse_iterator {
            return const_reverse_iterator(this->end());
        }

        // TODO: Need to test
        [[nodiscard]]
        constexpr auto rend() const noexcept -> const_reverse_iterator {
            return const_reverse_iterator(this->begin());
        }

        // TODO: Need to test
        [[nodiscard]]
        constexpr auto crbegin() const noexcept -> const_reverse_iterator {
            return this->rbegin();
        }

        // TODO: Need to test
        [[nodiscard]]
        constexpr auto crend() const noexcept -> const_reverse_iterator {
            return this->rend();
        }

        // TODO: Need to test
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto empty() const noexcept -> bool {
            return this->sz_ == 0;
        }

        // TODO: Need to test
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto size() const noexcept -> size_type {
            return this->sz_;
        }

        // TODO: Need to implement
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto max_size() const noexcept -> size_type {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr void reserve(const size_type new_cap) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to test
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr size_type capacity() const noexcept {
            return this->cap_;
        }

        // TODO: Need to implement
        constexpr void shrink_to_fit() {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr void clear() noexcept {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr iterator insert(const_iterator pos, const_reference value) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr iterator insert(const_iterator pos, value_type&& value) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr iterator insert(
            const_iterator pos,
            const size_type count,
            const_reference value
        ) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        template<std::input_iterator InputIt>
        constexpr iterator insert(
            const_iterator pos,
            InputIt first,
            InputIt last
        ) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr iterator insert(
            const_iterator pos,
            std::initializer_list<value_type> values
        ) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to timplement
        template<std::ranges::input_range R> requires (
            std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        )
        constexpr iterator insert_range(const_iterator pos, R&& rg) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        template<typename... Args>
        constexpr iterator emplace(const_iterator pos, Args&&... args) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr iterator erase(const_iterator pos) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr iterator erase(const_iterator first, const_iterator last) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr void push_back(const_reference value) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr void push_back(value_type&& value) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        template<typename... Args>
        constexpr reference emplace_back(Args&&... args) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        template<std::ranges::input_range R> requires(
            std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        )
        constexpr void append_range(R&& rg) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr void pop_back() {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr void resize(const size_type count) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr void resize(const size_type count, const_reference value) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr void swap(vector& other) noexcept(
            allocator_traits::propagate_on_container_swap::value ||
            allocator_traits::is_always_equal::value
        ) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to test
        constexpr void fill(
            const_reference value
        ) noexcept (
            std::is_nothrow_copy_assignable_v<value_type>
        ) { std::fill(this->begin(), this->end(), value); }

        // TODO: Need to test
        constexpr void swap(
            vector& other
        ) noexcept (
            noexcept(std::is_nothrow_swappable_v<value_type>)
        ) { std::swap(this->vals_, other.vals_); }

        // TODO: Need to test
        constexpr void sort() {
            this->_sort(this->begin(), this->end(), std::less<value_type>{});
        }

        // TODO: Need to test
        template<std::contiguous_iterator Iterator = iterator>
        constexpr void sort(Iterator first, Iterator last) {
            if (first == last) [[unlikely]] {
                return;
            }
            this->_sort(first, last, std::less<value_type>{});
        }

        // TODO: Need to test
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

        // TODO: Need to test
        constexpr void stable_sort() {
            this->_stable_sort(
                this->begin(),
                this->end(),
                std::less<value_type>{}
            );
        }

        // TODO: Need to test
        template<std::contiguous_iterator Iterator = iterator>
        constexpr void stable_sort(Iterator first, Iterator last) {
            if (first == last) [[unlikely]] {
                return;
            }
            this->_stable_sort(first, last, std::less<value_type>{});
        }

        // TODO: Need to test
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
    template<
        std::input_iterator InputIt,
        typename Alloc = std::allocator<
            typename std::iterator_traits<InputIt>::value_type
        >
    >
    circular_vector(InputIt, InputIt, Alloc = Alloc()) -> circular_vector<
        typename std::iterator_traits<InputIt>::value_type,
        Alloc
    >;

    template<
        std::ranges::input_range R,
        typename Alloc = std::allocator<std::ranges::range_value_t<R>>
    >
    circular_vector(
        std::from_range_t,
        R&&, Alloc = Alloc()
    ) -> circular_vector<
        std::ranges::range_value_t<R>,
        Alloc
    >;

    // ── Functions ───────────────────────────────────────────────────────────
    // TODO: Need to implement
    template<typename T, typename Alloc>
    constexpr void swap(
        circular_vector<T, Alloc>& lhs,
        circular_vector<T, Alloc>& rhs
    ) noexcept {
        throw std::runtime_error("Not implemented");
    }

    // TODO: Need to implement
    template<typename T, typename Alloc, typename U = T>
    constexpr circular_vector<T, Alloc>::size_type erase(
        circular_vector<T, Alloc>& vec,
        const U& value
    ) {
        throw std::runtime_error("Not implemented");
    }

    // TODO: Need to implement
    template<typename T, typename Alloc, std::predicate Pred>
    constexpr circular_vector<T, Alloc>::size_type erase_if(
        circular_vector<T, Alloc>& vec,
        const Pred pred
    ) {
        throw std::runtime_error("Not implemented");
    }
} // namespace collections
#endif // #ifndef COLLECTIONS_CIRCULAR_VECTOR_HPP
