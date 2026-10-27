#ifndef COLLECTIONS_FORWARD_LIST_HPP
#define COLLECTIONS_FORWARD_LIST_HPP

// ISO C Includes
#include <cstddef>

// ISO C++ Includes
#include <memory>

namespace collections {
    template<typename T, typename Allocator = std::allocator<T>>
    class forward_list {
    public:
        using value_type = T;

        using size_type = std::size_t;


    private:

    };
} // namespace collections

#endif // #ifndef COLLECTIONS_FORWARD_LIST_HPP
