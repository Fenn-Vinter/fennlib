#if !defined(__FENNLIB_ALLOW_INTERNAL__) && !defined(__clangd__) && !defined(__INTELLISENSE__) && !defined(__clang_analyzer__)
    #error "fennlib/cxx17/_syscall.hpp is an internal header! Include <syscall> or <fennlib> instead."
#endif

#if !defined(__FENNLIB_INTERNAL_CXX17_SYSCALL_HPP) && defined(__FENNLIB_ALLOW_INTERNAL__)
#define __FENNLIB_INTERNAL_CXX17_SYSCALL_HPP

#include "_types.hpp"
#include "_sys.hpp"

namespace fennlib::sys {

    namespace detail {

        inline isize raw_syscall6(usize sys_no, usize a1, usize a2, usize a3, usize a4, usize a5, usize a6) noexcept {
#if defined(__x86_64__) || defined(_M_X64) || defined(__amd64__)
            isize ret;
            register usize r10 __asm__("r10") = a4;
            register usize r8  __asm__("r8")  = a5;
            register usize r9  __asm__("r9")  = a6;
            __asm__ __volatile__(
                "syscall"
                : "=a"(ret)
                : "a"(sys_no), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8), "r"(r9)
                : "rcx", "r11", "memory"
            );
            return ret;

#elif defined(__aarch64__) || defined(_M_ARM64) || defined(__arm64__)
            register usize x8 __asm__("x8") = sys_no;
            register usize x0 __asm__("x0") = a1;
            register usize x1 __asm__("x1") = a2;
            register usize x2 __asm__("x2") = a3;
            register usize x3 __asm__("x3") = a4;
            register usize x4 __asm__("x4") = a5;
            register usize x5 __asm__("x5") = a6;
            __asm__ __volatile__(
                "svc #0"
                : "+r"(x0)
                : "r"(x8), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5)
                : "memory"
            );
            return static_cast<isize>(x0);

#elif defined(__i386__) || defined(_M_IX86)
            isize ret;
            __asm__ __volatile__(
                "int $0x80"
                : "=a"(ret)
                : "a"(sys_no), "b"(a1), "c"(a2), "d"(a3), "S"(a4), "D"(a5)
                : "memory"
            );
            return ret;

#elif defined(__riscv)
            register usize a7_reg __asm__("a7") = sys_no;
            register usize a0_reg __asm__("a0") = a1;
            register usize a1_reg __asm__("a1") = a2;
            register usize a2_reg __asm__("a2") = a3;
            register usize a3_reg __asm__("a3") = a4;
            register usize a4_reg __asm__("a4") = a5;
            register usize a5_reg __asm__("a5") = a6;
            __asm__ __volatile__(
                "ecall"
                : "+r"(a0_reg)
                : "r"(a7_reg), "r"(a1_reg), "r"(a2_reg), "r"(a3_reg), "r"(a4_reg), "r"(a5_reg)
                : "memory"
            );
            return static_cast<isize>(a0_reg);

#else
            (void)sys_no; (void)a1; (void)a2; (void)a3; (void)a4; (void)a5; (void)a6;
            return -1;
#endif
        }

    }

    template <usize SysNo, typename... Args>
    inline auto raw_syscall(Args... args) -> isize {
        static_assert(sizeof...(Args) <= 6, "fennlib::sys::raw_syscall accepts a maximum of 6 arguments!");

        auto to_reg = [](auto val) -> usize {
            if constexpr (sizeof(val) == sizeof(void*)) {
                return reinterpret_cast<usize>(val);
            } else {
                return static_cast<usize>(val);
            }
        };

        usize reg_args[6]{0, 0, 0, 0, 0, 0};
        usize idx = 0;
        
        if constexpr (sizeof...(Args) > 0) {
            ((reg_args[idx++] = to_reg(args)), ...);
        }

        return detail::raw_syscall6(
            SysNo,
            reg_args[0],
            reg_args[1],
            reg_args[2],
            reg_args[3],
            reg_args[4],
            reg_args[5]
        );
    }

}

#endif