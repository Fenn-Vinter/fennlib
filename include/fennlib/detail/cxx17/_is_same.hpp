#if !defined(__FENNLIB_ALLOW_INTERNAL__) && !defined(__clangd__) && !defined(__INTELLISENSE__) && !defined(__clang_analyzer__)
    #error "fennlib/cxx17/_is_same.hpp is an internal header! Include <is_same> or <fennlib> instead."
#endif

#if !defined(__FENNLIB_INTERNAL_CXX17_IS_SAME_HPP) && defined(__FENNLIB_ALLOW_INTERNAL__)
#define __FENNLIB_INTERNAL_CXX17_IS_SAME_HPP

#include "_types.hpp"

namespace fennlib {

    template <typename T, typename U>
    struct is_same { static constexpr bool value = false; };

    template <typename T>
    struct is_same<T, T> { static constexpr bool value = true; };

    template <typename T, typename U>
    inline constexpr bool is_same_v = is_same<T, U>::value;

}

#endif