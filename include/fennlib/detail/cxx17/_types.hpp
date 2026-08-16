#if !defined(__FENNLIB_ALLOW_INTERNAL__) && !defined(__clangd__) && !defined(__INTELLISENSE__) && !defined(__clang_analyzer__)
    #error "fennlib/cxx17/_types.hpp is an internal header! Include <types> or <fennlib> instead."
#endif

#if !defined(__FENNLIB_INTERNAL_CXX17_TYPES_HPP) && defined(__FENNLIB_ALLOW_INTERNAL__)
#define __FENNLIB_INTERNAL_CXX17_TYPES_HPP

#if defined(__UINT8_TYPE__)
    #define FENNLIB_U8 __UINT8_TYPE__
#elif defined(_MSC_VER)
    #define FENNLIB_U8 unsigned __int8
#else
    #define FENNLIB_U8 unsigned char
#endif

#if defined(__UINT16_TYPE__)
    #define FENNLIB_U16 __UINT16_TYPE__
#elif defined(_MSC_VER)
    #define FENNLIB_U16 unsigned __int16
#else
    #define FENNLIB_U16 unsigned short
#endif

#if defined(__UINT32_TYPE__)
    #define FENNLIB_U32 __UINT32_TYPE__
#elif defined(_MSC_VER)
    #define FENNLIB_U32 unsigned __int32
#else
    #define FENNLIB_U32 unsigned int
#endif

#if defined(__UINT64_TYPE__)
    #define FENNLIB_U64 __UINT64_TYPE__
#elif defined(_MSC_VER)
    #define FENNLIB_U64 unsigned __int64
#else
    #define FENNLIB_U64 unsigned long long int
#endif

#if defined(__INT8_TYPE__)
    #define FENNLIB_I8 __INT8_TYPE__
#elif defined(_MSC_VER)
    #define FENNLIB_I8 signed __int8
#else
    #define FENNLIB_I8 signed char
#endif

#if defined(__INT16_TYPE__)
    #define FENNLIB_I16 __INT16_TYPE__
#elif defined(_MSC_VER)
    #define FENNLIB_I16 signed __int16
#else
    #define FENNLIB_I16 signed short
#endif

#if defined(__INT32_TYPE__)
    #define FENNLIB_I32 __INT32_TYPE__
#elif defined(_MSC_VER)
    #define FENNLIB_I32 signed __int32
#else
    #define FENNLIB_I32 signed int
#endif

#if defined(__INT64_TYPE__)
    #define FENNLIB_I64 __INT64_TYPE__
#elif defined(_MSC_VER)
    #define FENNLIB_I64 signed __int64
#else
    #define FENNLIB_I64 signed long long int
#endif

#if defined(__SIZE_TYPE__)
    #define FENNLIB_USIZE __SIZE_TYPE__
#elif defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__)
    #define FENNLIB_USIZE FENNLIB_U64
#else
    #define FENNLIB_USIZE FENNLIB_U32
#endif

#if defined(__PTRDIFF_TYPE__)
    #define FENNLIB_ISIZE __PTRDIFF_TYPE__
#elif defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__)
    #define FENNLIB_ISIZE FENNLIB_I64
#else
    #define FENNLIB_ISIZE FENNLIB_I32
#endif

namespace fennlib::types {
    using u8  = FENNLIB_U8;
    using u16 = FENNLIB_U16;
    using u32 = FENNLIB_U32;
    using u64 = FENNLIB_U64;

    static_assert(sizeof(u8)  == 1, "fennlib::types::u8 does not match 1 byte");
    static_assert(sizeof(u16) == 2, "fennlib::types::u16 does not match 2 bytes");
    static_assert(sizeof(u32) == 4, "fennlib::types::u32 does not match 4 bytes");
    static_assert(sizeof(u64) == 8, "fennlib::types::u64 does not match 8 bytes");

    using i8  = FENNLIB_I8;
    using i16 = FENNLIB_I16;
    using i32 = FENNLIB_I32;
    using i64 = FENNLIB_I64;

    static_assert(sizeof(i8)  == 1, "fennlib::types::i8 does not match 1 byte");
    static_assert(sizeof(i16) == 2, "fennlib::types::i16 does not match 2 bytes");
    static_assert(sizeof(i32) == 4, "fennlib::types::i32 does not match 4 bytes");
    static_assert(sizeof(i64) == 8, "fennlib::types::i64 does not match 8 bytes");

    using usize = FENNLIB_USIZE;
    using isize = FENNLIB_ISIZE;

    using uintptr = usize;
    using intptr  = isize;

    using uint = unsigned int;
    static_assert(sizeof(uint) == sizeof(int), "fennlib::types::uint size does not match signed int");

    namespace internal {
        template <typename T>
        struct wild_wrapper {
            T value;

            constexpr wild_wrapper() noexcept : value(0) {}

            template <typename U>
            constexpr wild_wrapper(U val) noexcept {
                static_assert(sizeof(U) <= sizeof(T), "Type injected into wild type exceeds fixed byte capacity!");
                value = static_cast<T>(val);
            }

            template <typename U>
            wild_wrapper(U* ptr) noexcept {
                static_assert(sizeof(U*) <= sizeof(T), "Pointer injected into wild type exceeds fixed byte capacity!");
                value = static_cast<T>(reinterpret_cast<fennlib::types::usize>(ptr));
            }

            wild_wrapper(void* ptr) noexcept {
                value = static_cast<T>(reinterpret_cast<fennlib::types::usize>(ptr));
            }

            wild_wrapper(const void* ptr) noexcept {
                value = static_cast<T>(reinterpret_cast<fennlib::types::usize>(ptr));
            }

            template <typename U>
            constexpr operator U() const noexcept {
                return static_cast<U>(value);
            }

            template <typename U>
            explicit operator U*() const noexcept {
                return reinterpret_cast<U*>(static_cast<fennlib::types::usize>(value));
            }

            operator void*() const noexcept {
                return reinterpret_cast<void*>(static_cast<fennlib::types::usize>(value));
            }

            operator const void*() const noexcept {
                return reinterpret_cast<const void*>(static_cast<fennlib::types::usize>(value));
            }

            [[nodiscard]] constexpr fennlib::types::usize address() const noexcept {
                return static_cast<fennlib::types::usize>(value);
            }

            constexpr wild_wrapper& operator++() noexcept {
                ++value;
                return *this;
            }

            constexpr wild_wrapper& operator--() noexcept {
                --value;
                return *this;
            }

            constexpr wild_wrapper operator++(int) noexcept {
                wild_wrapper temp = *this;
                ++value;
                return temp;
            }

            constexpr wild_wrapper operator--(int) noexcept {
                wild_wrapper temp = *this;
                --value;
                return temp;
            }

            template <typename U>
            constexpr wild_wrapper operator+(U offset) const noexcept {
                return wild_wrapper(value + static_cast<T>(offset));
            }

            template <typename U>
            constexpr wild_wrapper operator-(U offset) const noexcept {
                return wild_wrapper(value - static_cast<T>(offset));
            }

            template <typename U>
            constexpr wild_wrapper& operator+=(U offset) noexcept {
                value += static_cast<T>(offset);
                return *this;
            }

            template <typename U>
            constexpr wild_wrapper& operator-=(U offset) noexcept {
                value -= static_cast<T>(offset);
                return *this;
            }

            constexpr T operator-(const wild_wrapper& other) const noexcept {
                return value - other.value;
            }

            template <typename U>
            [[nodiscard]] constexpr wild_wrapper offset(U byte_offset) const noexcept {
                return wild_wrapper(value + static_cast<T>(byte_offset));
            }

            constexpr bool operator==(const wild_wrapper& other) const noexcept {
                return value == other.value;
            }

            constexpr bool operator!=(const wild_wrapper& other) const noexcept {
                return value != other.value;
            }

            template <typename U>
            constexpr bool operator==(U val) const noexcept {
                return value == static_cast<T>(val);
            }

            template <typename U>
            constexpr bool operator!=(U val) const noexcept {
                return value != static_cast<T>(val);
            }
        };
    }

    template <typename T, typename U>
    constexpr internal::wild_wrapper<T> operator+(U offset, const internal::wild_wrapper<T>& ptr) noexcept {
        return ptr + offset;
    }

    using w8      = internal::wild_wrapper<u8>;
    using w16     = internal::wild_wrapper<u16>;
    using w32     = internal::wild_wrapper<u32>;
    using w64     = internal::wild_wrapper<u64>;
    using wild    = internal::wild_wrapper<uint>;
    using wildptr = internal::wild_wrapper<usize>;

    using f32 = float;
    using f64 = double;
    using f80 = long double;

    static_assert(sizeof(f32) == 4, "fennlib::types::f32 must be 4 bytes");
    static_assert(sizeof(f64) == 8, "fennlib::types::f64 must be 8 bytes");
    static_assert(sizeof(f80) >= sizeof(f64), "fennlib::types::f80 must be at least as large as f64");

    using db = w8;
    using dw = w16;
    using dd = w32;
    using dq = w64;

    using byte = w8;
    using bit  = bool;
}

namespace fennlib {
    using namespace types;
}

#undef FENNLIB_U8
#undef FENNLIB_U16
#undef FENNLIB_U32
#undef FENNLIB_U64

#undef FENNLIB_I8
#undef FENNLIB_I16
#undef FENNLIB_I32
#undef FENNLIB_I64

#undef FENNLIB_USIZE
#undef FENNLIB_ISIZE

#endif