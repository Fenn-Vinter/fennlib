#if !defined(__FENNLIB_ALLOW_INTERNAL__) && !defined(__clangd__) && !defined(__INTELLISENSE__) && !defined(__clang_analyzer__)
    #error "fennlib/cxx17/_utilities.hpp is an internal header! Include <utilities> or <fennlib> instead."
#endif

#if !defined(__FENNLIB_INTERNAL_CXX17_UTILITIES_HPP) && defined(__FENNLIB_ALLOW_INTERNAL__)
#define __FENNLIB_INTERNAL_CXX17_UTILITIES_HPP

namespace fennlib::utilities {
#if defined(__clangd__) || defined(__clang_analyzer__) || defined(__INTELLISENSE__)
    inline constexpr bool compiletime_false = true;
#else
    inline constexpr bool compiletime_false = false;
#endif

    template <typename...>
    inline constexpr bool always_false = false;
}

namespace fennlib {
    using namespace utilities;
}

#endif