#if !defined(__FENNLIB_ALLOW_INTERNAL__) && !defined(__clangd__) && !defined(__INTELLISENSE__) && !defined(__clang_analyzer__)
    #error "fennlib/cxx17/_args.hpp is an internal header! Include <args> or <fennlib> instead."
#endif

#if !defined(__FENNLIB_INTERNAL_CXX17_ARGS_HPP) && defined(__FENNLIB_ALLOW_INTERNAL__)
#define __FENNLIB_INTERNAL_CXX17_ARGS_HPP

#include "_types.hpp"
#include "_sequence.hpp"
#include "_is_same.hpp"

namespace fennlib::args {

#if defined(_WIN32)
    inline int get_argc() noexcept { return __argc; }
    inline char** get_argv() noexcept { return __argv; }

#elif defined(__linux__)
    namespace detail {
        inline int& linux_argc() noexcept {
            static int argc = 0;
            return argc;
        }
        inline char**& linux_argv() noexcept {
            static char** argv = nullptr;
            return argv;
        }

        __attribute__((constructor))
        inline void capture_args(int argc, char** argv) noexcept {
            linux_argc() = argc;
            linux_argv() = argv;
        }
    }

    inline int get_argc() noexcept { return detail::linux_argc(); }
    inline char** get_argv() noexcept { return detail::linux_argv(); }

#else
    inline int get_argc() noexcept { return 0; }
    inline char** get_argv() noexcept { return nullptr; }
#endif

    inline sequence<const char*> get_argv_sequence() {
        char** argv = get_argv();
        usize argc = static_cast<usize>(get_argc());
        sequence<const char*> result;
        if (argv) {
            for (usize i = 0; i < argc; ++i) {
                result.push_back(argv[i]);
            }
        }
        return result;
    }

    inline sequence<const char*> get_args() {
        sequence<const char*> argv_seq = get_argv_sequence();
        sequence<const char*> result;
        for (usize i = 1; i < argv_seq.size(); ++i) {
            result.push_back(argv_seq[i]);
        }
        return result;
    }
}

namespace fennlib {
    using namespace args;
}

#endif