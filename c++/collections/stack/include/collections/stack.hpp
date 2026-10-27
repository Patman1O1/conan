#ifndef COLLECTIONS_STACK_HPP
#define COLLECTIONS_STACK_HPP

// ISO C Includes
#include <cstddef>

// ISO C++ Includes
#include <iterator>
#include <ranges>

namespace collections {
    template<typename T, std::ranges::range Container>
    class stack {
    public:
        using value_type = T;

	using container_type = Container;

	using size_type = std::size_t;

	using difference_type = std::ptrdiff_t;

	using reference = value_type&;

	using const_reference = const value_type&;
    };
} // namespace collections

#endif // #ifndef COLLECTIONS_STACK_HPP
