#if !defined(__FENNLIB_ALLOW_INTERNAL__) && !defined(__clangd__) && !defined(__INTELLISENSE__) && !defined(__clang_analyzer__)
    #error "fennlib/cxx17/_new.hpp is an internal header! Include <new> or <fennlib> instead."
#endif

#if !defined(__FENNLIB_INTERNAL_CXX17_NEW_HPP) && defined(__FENNLIB_ALLOW_INTERNAL__)
#define __FENNLIB_INTERNAL_CXX17_NEW_HPP

#include "_types.hpp"

namespace fennlib {
    struct placement_tag {};
    inline constexpr placement_tag place{};
} // namespace fennlib

// Tagged placement overloads avoid any collision with standard operator new signatures
inline void* operator new(decltype(sizeof(0)), fennlib::placement_tag, void* __p) noexcept {
    return __p;
}

inline void* operator new[](decltype(sizeof(0)), fennlib::placement_tag, void* __p) noexcept {
    return __p;
}

inline void operator delete(void*, fennlib::placement_tag, void*) noexcept {}
inline void operator delete[](void*, fennlib::placement_tag, void*) noexcept {}

namespace fennlib {

template <typename T>
struct is_trivially_copyable {
#if defined(_MSC_VER) && !defined(__clang__)
    static constexpr bool value = __is_trivially_copyable(T);
#elif defined(__GNUG__) || defined(__clang__)
    static constexpr bool value = __is_trivially_copyable(T);
#else
    static constexpr bool value = false;
#endif
};

template <typename T, typename... Args>
inline T* construct(void* __p, Args&&... args) {
    if constexpr (sizeof...(Args) == 1 && is_trivially_copyable<T>::value) {
        *static_cast<T*>(__p) = T(static_cast<Args&&>(args)...);
        return static_cast<T*>(__p);
    } else {
        return ::new (::fennlib::place, __p) T(static_cast<Args&&>(args)...);
    }
}

template <typename T>
inline void destroy(T* __p) noexcept {
    if (__p) __p->~T();
}

} // namespace fennlib

#endif