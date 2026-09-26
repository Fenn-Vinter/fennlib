#if !defined(__FENNLIB_ALLOW_INTERNAL__) && !defined(__clangd__) && !defined(__INTELLISENSE__) && !defined(__clang_analyzer__)
    #error "fennlib/cxx17/_new.hpp is an internal header! Include <new> or <fennlib> instead."
#endif

#if !defined(__FENNLIB_INTERNAL_CXX17_NEW_HPP) && defined(__FENNLIB_ALLOW_INTERNAL__)
#define __FENNLIB_INTERNAL_CXX17_NEW_HPP
#include "_types.hpp"

// Use standard definitions for placement new to satisfy Clang's compiler diagnostics
inline void* operator new(decltype(sizeof(0)), void* __p) noexcept {
    return __p;
}

inline void* operator new[](decltype(sizeof(0)), void* __p) noexcept {
    return __p;
}

inline void operator delete(void*, void*) noexcept {}
inline void operator delete[](void*, void*) noexcept {}

#endif