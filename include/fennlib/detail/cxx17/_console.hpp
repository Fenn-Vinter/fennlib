#if !defined(__FENNLIB_ALLOW_INTERNAL__) && !defined(__clangd__) && !defined(__INTELLISENSE__) && !defined(__clang_analyzer__)
    #error "fennlib/cxx17/_console.hpp is an internal header! Include <console> or <fennlib> instead."
#endif

#if !defined(__FENNLIB_INTERNAL_CXX17_CONSOLE_HPP) && defined(__FENNLIB_ALLOW_INTERNAL__)
#define __FENNLIB_INTERNAL_CXX17_CONSOLE_HPP

#if defined(_WIN32) || defined(_WIN64)
extern "C" {
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long nStdHandle);
    __declspec(dllimport) int __stdcall WriteFile(
        void* hFile, 
        const void* lpBuffer, 
        unsigned long nNumberOfBytesToWrite, 
        unsigned long* lpNumberOfBytesWritten, 
        void* lpOverlapped
    );
    __declspec(dllimport) int __stdcall ReadFile(
        void* hFile,
        void* lpBuffer,
        unsigned long nNumberOfBytesToRead,
        unsigned long* lpNumberOfBytesRead,
        void* lpOverlapped
    );
}
#endif

#include "_types.hpp"
#include "_sys.hpp"
#include "_syscall.hpp"

namespace fennlib::console::internal {

    template <typename...>
    using void_t = void;

    inline isize write_stdout(const char* data, usize size) noexcept {
        if (!data || size == 0) return 0;

        if constexpr (fennlib::sys::os::is_linux) {
#if defined(__x86_64__)
            constexpr usize sys_write = 1;
#elif defined(__aarch64__)
            constexpr usize sys_write = 64;
#elif defined(__i386__)
            constexpr usize sys_write = 4;
#else
            constexpr usize sys_write = 1;
#endif
            return fennlib::sys::raw_syscall<sys_write>(1, data, size);
        }
        else if constexpr (fennlib::sys::os::is_windows) {
            constexpr unsigned long std_output_handle = static_cast<unsigned long>(-11);
            
            fennlib::types::wildptr handle = GetStdHandle(std_output_handle);

            constexpr fennlib::types::usize invalid_handle = ~static_cast<fennlib::types::usize>(0);
            if (handle.address() == 0 || handle.address() == invalid_handle) {
                return -1;
            }

            unsigned long bytes_written{0};
            if (WriteFile(handle, data, static_cast<unsigned long>(size), &bytes_written, nullptr)) {
                return static_cast<isize>(bytes_written);
            }
            return -1;
        }
        else if constexpr (fennlib::sys::os::is_apple) {
            constexpr usize sys_write = 0x2000004; 
            return fennlib::sys::raw_syscall<sys_write>(1, data, size);
        }
        else {
            return -1;
        }
    }

    inline isize read_stdin(char* buffer, usize max_size) noexcept {
        if (!buffer || max_size == 0) return 0;

        if constexpr (fennlib::sys::os::is_linux) {
#if defined(__x86_64__)
            constexpr usize sys_read = 0;
#elif defined(__aarch64__)
            constexpr usize sys_read = 63;
#elif defined(__i386__)
            constexpr usize sys_read = 3;
#else
            constexpr usize sys_read = 0;
#endif
            return fennlib::sys::raw_syscall<sys_read>(0, buffer, max_size);
        }
        else if constexpr (fennlib::sys::os::is_windows) {
            constexpr unsigned long std_input_handle = static_cast<unsigned long>(-10);
            
            fennlib::types::wildptr handle = GetStdHandle(std_input_handle);

            constexpr fennlib::types::usize invalid_handle = ~static_cast<fennlib::types::usize>(0);
            if (handle.address() == 0 || handle.address() == invalid_handle) {
                return -1;
            }

            unsigned long bytes_read{0};
            if (ReadFile(handle, buffer, static_cast<unsigned long>(max_size), &bytes_read, nullptr)) {
                return static_cast<isize>(bytes_read);
            }
            return -1;
        }
        else if constexpr (fennlib::sys::os::is_apple) {
            constexpr usize sys_read = 0x2000003; 
            return fennlib::sys::raw_syscall<sys_read>(0, buffer, max_size);
        }
        else {
            return -1;
        }
    }

    template <typename T>
    inline void print_int(T val) noexcept {
        if (val == 0) {
            write_stdout("0", 1);
            return;
        }

        char buffer[32];
        usize pos = 32;
        bool negative = false;

        if constexpr (static_cast<T>(-1) < static_cast<T>(0)) {
            if (val < 0) {
                negative = true;
                val = -val;
            }
        }

        auto uval = static_cast<fennlib::types::u64>(val);

        while (uval > 0) {
            buffer[--pos] = static_cast<char>('0' + (uval % 10));
            uval /= 10;
        }

        if (negative) {
            buffer[--pos] = '-';
        }

        write_stdout(&buffer[pos], 32 - pos);
    }

    template <typename T, typename = void>
    struct has_data_and_size {
        static constexpr bool value = false;
    };

    template <typename T>
    struct has_data_and_size<T, void_t<
        decltype(static_cast<const char*>(static_cast<const T*>(nullptr)->data())),
        decltype(static_cast<fennlib::types::usize>(static_cast<const T*>(nullptr)->size()))
    >> {
        static constexpr bool value = true;
    };

}

namespace fennlib::console {

    template <typename... Args>
    inline void print(const Args&... args) noexcept;

    inline void print(const char* str) noexcept {
        if (!str) {
            internal::write_stdout("(null)", 6);
            return;
        }
        usize len = 0;
        while (str[len] != '\0') ++len;
        internal::write_stdout(str, len);
    }

    inline void print(char c) noexcept {
        internal::write_stdout(&c, 1);
    }

    inline void print(bool b) noexcept {
        if (b) {
            internal::write_stdout("true", 4);
        } else {
            internal::write_stdout("false", 5);
        }
    }

    inline void print(i8 val) noexcept   { internal::print_int(val); }
    inline void print(u8 val) noexcept   { internal::print_int(val); }
    inline void print(i16 val) noexcept  { internal::print_int(val); }
    inline void print(u16 val) noexcept  { internal::print_int(val); }
    inline void print(i32 val) noexcept  { internal::print_int(val); }
    inline void print(u32 val) noexcept  { internal::print_int(val); }
    inline void print(i64 val) noexcept  { internal::print_int(val); }
    inline void print(u64 val) noexcept  { internal::print_int(val); }

    inline void print(wildptr ptr) noexcept {
        internal::write_stdout("0x", 2);
        
        usize val = ptr.address();
        if (val == 0) {
            internal::write_stdout("0", 1);
            return;
        }

        char buffer[16];
        usize pos = 16;
        constexpr const char* hex_digits = "0123456789abcdef";

        while (val > 0) {
            buffer[--pos] = hex_digits[val & 0xF];
            val >>= 4;
        }

        internal::write_stdout(&buffer[pos], 16 - pos);
    }

    template <typename T>
    inline void print(const T& val) noexcept {
        if constexpr (internal::has_data_and_size<T>::value) {
            internal::write_stdout(val.data(), val.size());
        }
        else if constexpr (sizeof(T*) == sizeof(usize)) {
            print(wildptr(val));
        }
    }

    template <typename First, typename Second, typename... Rest>
    inline void print(const First& first, const Second& second, const Rest&... rest) noexcept {
        print(first);
        print(second);
        (print(rest), ...);
    }

    template <typename... Args>
    inline void println(const Args&... args) noexcept {
        if constexpr (sizeof...(args) > 0) {
            print(args...);
        }
        internal::write_stdout("\n", 1);
    }

}

#endif