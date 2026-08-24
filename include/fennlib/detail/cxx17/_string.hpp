#if !defined(__FENNLIB_ALLOW_INTERNAL__) && !defined(__clangd__) && !defined(__INTELLISENSE__) && !defined(__clang_analyzer__)
    #error "fennlib/cxx17/_string.hpp is an internal header! Include <string> or <fennlib> instead."
#endif

#if !defined(__FENNLIB_INTERNAL_CXX17_STRING_HPP) && defined(__FENNLIB_ALLOW_INTERNAL__)
#define __FENNLIB_INTERNAL_CXX17_STRING_HPP

#include "_types.hpp"
#include "_is_same.hpp"

namespace fennlib {
    namespace internal {

        template <typename T>
        constexpr T declval_impl() noexcept;

        template <bool B, typename T = void>
        struct _enable_if {};

        template <typename T>
        struct _enable_if<true, T> { using type = T; };

        template <bool B, typename T = void>
        using _enable_if_t = typename _enable_if<B, T>::type;

        template <typename T>
        struct _decay_impl { using type = T; };
        
        template <typename T>
        struct _decay_impl<T&> { using type = T; };

        template <typename T>
        struct _decay_impl<const T> { using type = T; };

        template <typename T>
        using _decay_t = typename _decay_impl<T>::type;

        template <typename T>
        struct _has_begin_end {
        private:
            template <typename U>
            static constexpr auto test(int) -> decltype(
                declval_impl<const U&>().begin(),
                declval_impl<const U&>().end(),
                bool{}
            ) { return true; }

            template <typename>
            static constexpr bool test(...) { return false; }

        public:
            static constexpr bool value = test<T>(0);
        };

        template <fennlib::types::uint SSO = 23>
        class __basic_string {
        private:
            union {
                char* m_heap_data;
                char m_sso_data[SSO + 1];
            };

            usize m_size{0};
            usize m_capacity{SSO};
            bool m_is_heap{false};

            static constexpr usize c_str_len(const char* str) noexcept {
                if (!str) return 0;
                usize len = 0;
                while (str[len] != '\0') {
                    ++len;
                }
                return len;
            }

            void allocate_heap(usize capacity) {
                m_heap_data = static_cast<char*>(::operator new(capacity + 1));
                m_capacity = capacity;
                m_is_heap = true;
            }

            void free_heap() noexcept {
                if (m_is_heap && m_heap_data) {
                    ::operator delete(m_heap_data);
                    m_heap_data = nullptr;
                }
            }

        public:
            constexpr __basic_string() noexcept {
                m_sso_data[0] = '\0';
            }

            __basic_string(const char* str) {
                usize len = c_str_len(str);
                m_size = len;

                if (len <= SSO) {
                    m_is_heap = false;
                    m_capacity = SSO;
                    for (usize i = 0; i < len; ++i) {
                        m_sso_data[i] = str[i];
                    }
                    m_sso_data[len] = '\0';
                } else {
                    allocate_heap(len);
                    for (usize i = 0; i < len; ++i) {
                        m_heap_data[i] = str[i];
                    }
                    m_heap_data[len] = '\0';
                }
            }

            __basic_string(const __basic_string& other) {
                m_size = other.m_size;
                if (other.m_is_heap) {
                    allocate_heap(other.m_capacity);
                    for (usize i = 0; i <= m_size; ++i) {
                        m_heap_data[i] = other.m_heap_data[i];
                    }
                } else {
                    m_is_heap = false;
                    m_capacity = SSO;
                    for (usize i = 0; i <= m_size; ++i) {
                        m_sso_data[i] = other.m_sso_data[i];
                    }
                }
            }

            __basic_string(__basic_string&& other) noexcept {
                m_size = other.m_size;
                m_capacity = other.m_capacity;
                m_is_heap = other.m_is_heap;

                if (other.m_is_heap) {
                    m_heap_data = other.m_heap_data;
                    other.m_heap_data = nullptr;
                } else {
                    for (usize i = 0; i <= m_size; ++i) {
                        m_sso_data[i] = other.m_sso_data[i];
                    }
                }

                other.m_size = 0;
                other.m_capacity = SSO;
                other.m_is_heap = false;
                other.m_sso_data[0] = '\0';
            }

            ~__basic_string() noexcept {
                free_heap();
            }

            __basic_string& operator=(const char* str) {
                free_heap();
                usize len = c_str_len(str);
                m_size = len;

                if (len <= SSO) {
                    m_is_heap = false;
                    m_capacity = SSO;
                    for (usize i = 0; i < len; ++i) {
                        m_sso_data[i] = str[i];
                    }
                    m_sso_data[len] = '\0';
                } else {
                    allocate_heap(len);
                    for (usize i = 0; i < len; ++i) {
                        m_heap_data[i] = str[i];
                    }
                    m_heap_data[len] = '\0';
                }
                return *this;
            }

            __basic_string(const char* first, const char* last) {
                if (!first || last < first) {
                    m_sso_data[0] = '\0';
                    return;
                }

                usize len = static_cast<usize>(last - first);
                m_size = len;

                if (len <= SSO) {
                    m_is_heap = false;
                    m_capacity = SSO;
                    for (usize i = 0; i < len; ++i) {
                        m_sso_data[i] = first[i];
                    }
                    m_sso_data[len] = '\0';
                } else {
                    allocate_heap(len);
                    for (usize i = 0; i < len; ++i) {
                        m_heap_data[i] = first[i];
                    }
                    m_heap_data[len] = '\0';
                }
            }

            template <typename Range,
                      typename = _enable_if_t<_has_begin_end<Range>::value && 
                                             !fennlib::is_same_v<_decay_t<Range>, __basic_string>>>
            __basic_string(const Range& range)
                : __basic_string(range.begin(), range.end()) {}

            template <typename Range,
                      typename = _enable_if_t<_has_begin_end<Range>::value && 
                                             !fennlib::is_same_v<_decay_t<Range>, __basic_string>>>
            __basic_string& operator=(const Range& range) {
                *this = __basic_string(range.begin(), range.end());
                return *this;
            }

            [[nodiscard]] const char* c_str() const noexcept {
                return m_is_heap ? m_heap_data : m_sso_data;
            }

            [[nodiscard]] char* data() noexcept {
                return m_is_heap ? m_heap_data : m_sso_data;
            }

            [[nodiscard]] const char* data() const noexcept {
                return m_is_heap ? m_heap_data : m_sso_data;
            }

            [[nodiscard]] constexpr usize size() const noexcept { return m_size; }
            [[nodiscard]] constexpr usize capacity() const noexcept { return m_capacity; }
            [[nodiscard]] constexpr bool empty() const noexcept { return m_size == 0; }
            [[nodiscard]] constexpr bool is_heap_allocated() const noexcept { return m_is_heap; }

            [[nodiscard]] char& operator[](usize index) noexcept { return data()[index]; }
            [[nodiscard]] const char& operator[](usize index) const noexcept { return data()[index]; }

            [[nodiscard]] char* begin() noexcept { return data(); }
            [[nodiscard]] const char* begin() const noexcept { return data(); }
            [[nodiscard]] char* end() noexcept { return data() + m_size; }
            [[nodiscard]] const char* end() const noexcept { return data() + m_size; }
        };
    }

    using string     = fennlib::internal::__basic_string<23>;
    using string8    = fennlib::internal::__basic_string<8>;
    using string16   = fennlib::internal::__basic_string<16>;
    using string32   = fennlib::internal::__basic_string<32>;
    using string64   = fennlib::internal::__basic_string<64>;
    using string128  = fennlib::internal::__basic_string<128>;
    using string256  = fennlib::internal::__basic_string<256>;
    using string512  = fennlib::internal::__basic_string<512>;
    using string1024 = fennlib::internal::__basic_string<1024>;
    using string2048 = fennlib::internal::__basic_string<2048>;
    using string4096 = fennlib::internal::__basic_string<4096>;
    using string8192 = fennlib::internal::__basic_string<8192>;
}

#endif