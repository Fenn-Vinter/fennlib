#if !defined(__FENNLIB_ALLOW_INTERNAL__) && !defined(__clangd__) && !defined(__INTELLISENSE__) && !defined(__clang_analyzer__)
    #error "fennlib/cxx17/_array.hpp is an internal header! Include <array> or <fennlib> instead."
#endif

#ifndef FENNLIB_INTERNAL_CXX17_ARRAY_HPP
#define FENNLIB_INTERNAL_CXX17_ARRAY_HPP

#include "_types.hpp"
#include "_is_same.hpp"

namespace fennlib {

    template <typename T, usize N>
    class array {
    private:
        T m_data[N]{};

    public:
        constexpr array() noexcept = default;

        template <typename... Args>
        constexpr array(Args... args) noexcept : m_data{ static_cast<T>(args)... } {
            static_assert(sizeof...(Args) <= N, "Too many arguments provided to fennlib::array constructor!");
        }

        template <usize StrLen>
        constexpr array(const char (&str)[StrLen]) noexcept {
            static_assert(fennlib::is_same_v<T, char>, "fennlib::array C-string constructor is only valid for T = char");
            static_assert(StrLen - 1 <= N, "String literal length exceeds fennlib::array capacity!");

            for (usize i = 0; i < StrLen - 1; ++i) {
                m_data[i] = str[i];
            }
            for (usize i = StrLen - 1; i < N; ++i) {
                m_data[i] = '\0';
            }
        }
        
        ~array() noexcept = default;
        
        [[nodiscard]] constexpr usize size() const noexcept { return N; }
        [[nodiscard]] constexpr usize capacity() const noexcept { return N; }
        [[nodiscard]] constexpr bool empty() const noexcept { return N == 0; }
        
        [[nodiscard]] constexpr T* data() noexcept { return m_data; }
        [[nodiscard]] constexpr const T* data() const noexcept { return m_data; }

        [[nodiscard]] constexpr T& operator[](usize index) noexcept { return m_data[index]; }
        [[nodiscard]] constexpr const T& operator[](usize index) const noexcept { return m_data[index]; }

        [[nodiscard]] constexpr T* begin() noexcept { return m_data; }
        [[nodiscard]] constexpr const T* begin() const noexcept { return m_data; }
        [[nodiscard]] constexpr T* end() noexcept { return m_data + N; }
        [[nodiscard]] constexpr const T* end() const noexcept { return m_data + N; }
    };

    template <typename T, usize N>
    using arr = array<T, N>;
} 

#endif