#ifndef __FENNLIB_INTERNAL_CXX17_SYS_HPP
#define __FENNLIB_INTERNAL_CXX17_SYS_HPP

#if !defined(__FENNLIB_ALLOW_INTERNAL__) && !defined(__clangd__) && !defined(__INTELLISENSE__) && !defined(__clang_analyzer__)
    #error "fennlib/cxx17/_sys.hpp is an internal header! Include <sys> or <fennlib> instead."
#endif

namespace fennlib::sys {

    namespace os {
#if defined(__AlopexOS__) || defined(__ALOPEXOS__)
        [[maybe_unused]] inline constexpr bool is_alopexos = true;
#else
        [[maybe_unused]] inline constexpr bool is_alopexos = false;
#endif

#if defined(__WINDOWS__) || defined(_WIN32) || defined(_WIN64)
        [[maybe_unused]] inline constexpr bool is_windows = true;
#else
        [[maybe_unused]] inline constexpr bool is_windows = false;
#endif

#if defined(__ANDROID__)
        [[maybe_unused]] inline constexpr bool is_android = true;
#else
        [[maybe_unused]] inline constexpr bool is_android = false;
#endif

#if defined(__APPLE__) && defined(__MACH__)
        [[maybe_unused]] inline constexpr bool is_apple = true;
#else
        [[maybe_unused]] inline constexpr bool is_apple = false;
#endif

#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
        [[maybe_unused]] inline constexpr bool is_freebsd = true;
#else
        [[maybe_unused]] inline constexpr bool is_freebsd = false;
#endif

#if defined(__OpenBSD__)
        [[maybe_unused]] inline constexpr bool is_openbsd = true;
#else
        [[maybe_unused]] inline constexpr bool is_openbsd = false;
#endif

#if defined(__NetBSD__)
        [[maybe_unused]] inline constexpr bool is_netbsd = true;
#else
        [[maybe_unused]] inline constexpr bool is_netbsd = false;
#endif

#if defined(__DragonFly__)
        [[maybe_unused]] inline constexpr bool is_dragonfly = true;
#else
        [[maybe_unused]] inline constexpr bool is_dragonfly = false;
#endif

#if defined(sun) || defined(__sun)
        [[maybe_unused]] inline constexpr bool is_solaris = true;
#else
        [[maybe_unused]] inline constexpr bool is_solaris = false;
#endif

#if defined(__HAIKU__)
        [[maybe_unused]] inline constexpr bool is_haiku = true;
#else
        [[maybe_unused]] inline constexpr bool is_haiku = false;
#endif

#if defined(__EMSCRIPTEN__)
        [[maybe_unused]] inline constexpr bool is_emscripten = true;
#else
        [[maybe_unused]] inline constexpr bool is_emscripten = false;
#endif

#if defined(_AIX) || defined(__aix__)
        [[maybe_unused]] inline constexpr bool is_aix = true;
#else
        [[maybe_unused]] inline constexpr bool is_aix = false;
#endif

#if (defined(__linux__) || defined(__LINUX__)) && !defined(__ANDROID__)
        [[maybe_unused]] inline constexpr bool is_linux = true;
#else
        [[maybe_unused]] inline constexpr bool is_linux = false;
#endif
    }

    namespace arch {
#if defined(__x86_64__) || defined(_M_X64) || defined(__amd64__)
        [[maybe_unused]] inline constexpr bool is_x64 = true;
        [[maybe_unused]] inline constexpr bool is_x86_64 = true;
        [[maybe_unused]] inline constexpr bool is_amd64 = true;
#else
        [[maybe_unused]] inline constexpr bool is_x64 = false;
        [[maybe_unused]] inline constexpr bool is_x86_64 = false;
        [[maybe_unused]] inline constexpr bool is_amd64 = false;
#endif

#if defined(__i386__) || defined(_M_IX86) || defined(_X86_) || defined(__THW_INTEL__)
        [[maybe_unused]] inline constexpr bool is_x86 = true;
        [[maybe_unused]] inline constexpr bool is_i386 = true;
#else
        [[maybe_unused]] inline constexpr bool is_x86 = false;
        [[maybe_unused]] inline constexpr bool is_i386 = false;
#endif

#if defined(__aarch64__) || defined(_M_ARM64) || defined(__arm64__)
        [[maybe_unused]] inline constexpr bool is_arm64 = true;
        [[maybe_unused]] inline constexpr bool is_aarch64 = true;
#else
        [[maybe_unused]] inline constexpr bool is_arm64 = false;
        [[maybe_unused]] inline constexpr bool is_aarch64 = false;
#endif

#if defined(__arm__) || defined(_M_ARM)
        [[maybe_unused]] inline constexpr bool is_arm32 = true;
        [[maybe_unused]] inline constexpr bool is_arm = true;
#else
        [[maybe_unused]] inline constexpr bool is_arm32 = false;
        [[maybe_unused]] inline constexpr bool is_arm = false;
#endif

#if defined(__riscv) && (__riscv_xlen == 64)
        [[maybe_unused]] inline constexpr bool is_riscv64 = true;
#else
        [[maybe_unused]] inline constexpr bool is_riscv64 = false;
#endif

#if defined(__riscv) && (__riscv_xlen == 32)
        [[maybe_unused]] inline constexpr bool is_riscv32 = true;
#else
        [[maybe_unused]] inline constexpr bool is_riscv32 = false;
#endif

#if defined(__ppc64__) || defined(__PPC64__) || defined(_ARCH_PPC64)
        [[maybe_unused]] inline constexpr bool is_ppc64 = true;
#else
        [[maybe_unused]] inline constexpr bool is_ppc64 = false;
#endif

#if defined(__powerpc__) || defined(__ppc__) || defined(_ARCH_PPC) || defined(_M_PPC)
        [[maybe_unused]] inline constexpr bool is_ppc32 = true;
#else
        [[maybe_unused]] inline constexpr bool is_ppc32 = false;
#endif

#if defined(__mips64) || defined(__mips64__) || defined(__mips_n64)
        [[maybe_unused]] inline constexpr bool is_mips64 = true;
#else
        [[maybe_unused]] inline constexpr bool is_mips64 = false;
#endif

#if defined(__mips__) || defined(__mips) || defined(__MIPS__) || defined(_M_MIPS)
        [[maybe_unused]] inline constexpr bool is_mips32 = true;
#else
        [[maybe_unused]] inline constexpr bool is_mips32 = false;
#endif

#if defined(__sparc_v9__) || defined(__sparcv9) || defined(__sparc64__)
        [[maybe_unused]] inline constexpr bool is_sparc64 = true;
#else
        [[maybe_unused]] inline constexpr bool is_sparc64 = false;
#endif

#if defined(__sparc__) || defined(__sparc)
        [[maybe_unused]] inline constexpr bool is_sparc32 = true;
#else
        [[maybe_unused]] inline constexpr bool is_sparc32 = false;
#endif

#if defined(__s390x__) || defined(__zarch__)
        [[maybe_unused]] inline constexpr bool is_s390x = true;
#else
        [[maybe_unused]] inline constexpr bool is_s390x = false;
#endif

#if defined(__s390__)
        [[maybe_unused]] inline constexpr bool is_s390 = true;
#else
        [[maybe_unused]] inline constexpr bool is_s390 = false;
#endif

#if defined(__ia64__) || defined(_M_IA64) || defined(__itanium__)
        [[maybe_unused]] inline constexpr bool is_ia64 = true;
#else
        [[maybe_unused]] inline constexpr bool is_ia64 = false;
#endif

#if defined(__sh__)
        [[maybe_unused]] inline constexpr bool is_superh = true;
#else
        [[maybe_unused]] inline constexpr bool is_superh = false;
#endif

#if defined(__m68k__) || defined(M68000)
        [[maybe_unused]] inline constexpr bool is_m68k = true;
#else
        [[maybe_unused]] inline constexpr bool is_m68k = false;
#endif

#if defined(__wasm64__)
        [[maybe_unused]] inline constexpr bool is_wasm64 = true;
#else
        [[maybe_unused]] inline constexpr bool is_wasm64 = false;
#endif

#if defined(__wasm__) || defined(__wasm32__)
        [[maybe_unused]] inline constexpr bool is_wasm32 = true;
#else
        [[maybe_unused]] inline constexpr bool is_wasm32 = false;
#endif
    }

    namespace bitsize {
        [[maybe_unused]] inline constexpr bool is_64 = (sizeof(void*) == 8);
        [[maybe_unused]] inline constexpr bool is_32 = (sizeof(void*) == 4);
        [[maybe_unused]] inline constexpr bool is_16 = (sizeof(void*) == 2);
        [[maybe_unused]] inline constexpr bool is_8  = (sizeof(void*) == 1);
    }

}

namespace fennlib {
#if defined(__clang__) || defined(__clangd__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunknown-pragmas"
    #pragma clang diagnostic ignored "-Wunknown-warning-option"
    #pragma clang diagnostic ignored "-Wunused"
    #pragma clang diagnostic ignored "-Wunused-local-typedef"
    #pragma clang diagnostic ignored "-Wunneeded-internal-declaration"
#elif defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpragmas"
    #pragma GCC diagnostic ignored "-Wunused-variable"
#elif defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable: 4189)
#endif

    namespace os = sys::os;            // NOLINT(misc-unused-alias-decls)
    namespace arch = sys::arch;        // NOLINT(misc-unused-alias-decls)
    namespace bitsize = sys::bitsize;  // NOLINT(misc-unused-alias-decls)

#if defined(__clang__) || defined(__clangd__)
    #pragma clang diagnostic pop
#elif defined(__GNUC__)
    #pragma GCC diagnostic pop
#elif defined(_MSC_VER)
    #pragma warning(pop)
#endif
}

#endif