#if !defined(__FENNLIB_ALLOW_INTERNAL__) && !defined(__clangd__) && !defined(__INTELLISENSE__) && !defined(__clang_analyzer__)
    #error "fennlib/cxx17/_new.hpp is an internal header! Include <new> or <fennlib> instead."
#endif

#if !defined(__FENNLIB_INTERNAL_CXX17_NEW_HPP) && defined(__FENNLIB_ALLOW_INTERNAL__)
#define __FENNLIB_INTERNAL_CXX17_NEW_HPP
#include "_types.hpp"

inline void* operator new(fennlib::types::usize, void* ptr) noexcept {
    return ptr;
}

inline void* operator new(fennlib::types::usize, fennlib::types::wildptr ptr) noexcept {
    return static_cast<void*>(ptr);
}

#endif