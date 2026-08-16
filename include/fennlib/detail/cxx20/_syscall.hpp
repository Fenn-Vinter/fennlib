#if !defined(__FENNLIB_ALLOW_INTERNAL__) && !defined(__clangd__) && !defined(__INTELLISENSE__) && !defined(__clang_analyzer__)
    #error "fennlib/cxx20/_syscall.hpp is an internal header! Include <syscall> or <fennlib> instead."
#endif

#if !defined(__FENNLIB_INTERNAL_CXX20_SYSCALL_HPP) && defined(__FENNLIB_ALLOW_INTERNAL__)
#define __FENNLIB_INTERNAL_CXX20_SYSCALL_HPP

#include "../cxx17/_syscall.hpp"

#endif