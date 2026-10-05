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

        inline bool string_equals(const char* a, const char* b) {
            if (!a || !b) return false;
            usize i = 0;
            while (a[i] != '\0' && b[i] != '\0') {
                if (a[i] != b[i]) return false;
                ++i;
            }
            return a[i] == b[i];
        }

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

        template <typename T>
        struct _is_char_type {
            static constexpr bool value = fennlib::is_same_v<_decay_t<T>, char> ||
                                          fennlib::is_same_v<_decay_t<T>, const char> ||
                                          fennlib::is_same_v<_decay_t<T>, char*> ||
                                          fennlib::is_same_v<_decay_t<T>, const char*>;
        };

        template <typename Range>
        struct _range_value_is_char {
        private:
            template <typename U>
            static constexpr auto test(int) -> decltype(
                _is_char_type<decltype(*declval_impl<const U&>().begin())>::value,
                bool{}
            ) { return _is_char_type<decltype(*declval_impl<const U&>().begin())>::value; }

            template <typename>
            static constexpr bool test(...) { return false; }

        public:
            static constexpr bool value = test<Range>(0);
        };

        template <fennlib::types::uint SSO = 23>
        class __basic_string {
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
            static constexpr usize npos = ~static_cast<usize>(0);

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

            __basic_string& operator=(const __basic_string& other) {
                if (this == &other) return *this;
                free_heap();
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
                return *this;
            }

            __basic_string& operator=(__basic_string&& other) noexcept {
                if (this == &other) return *this;
                free_heap();

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

                return *this;
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
                                              _range_value_is_char<Range>::value &&
                                              !fennlib::is_same_v<_decay_t<Range>, __basic_string>>>
            __basic_string(const Range& range)
                : __basic_string(range.begin(), range.end()) {}

            template <typename Range,
                      typename = _enable_if_t<_has_begin_end<Range>::value && 
                                              _range_value_is_char<Range>::value &&
                                              !fennlib::is_same_v<_decay_t<Range>, __basic_string>>>
            __basic_string& operator=(const Range& range) {
                *this = __basic_string(range.begin(), range.end());
                return *this;
            }

            [[nodiscard]] __basic_string substr(usize pos = 0, usize count = npos) const {
                if (pos > m_size) {
                    pos = m_size;
                }
                usize rlen = m_size - pos;
                if (count < rlen) {
                    rlen = count;
                }
                const char* base = data();
                return __basic_string(base + pos, base + pos + rlen);
            }

            __basic_string& erase(usize pos = 0, usize count = npos) {
                if (pos > m_size) {
                    pos = m_size;
                }
                usize rlen = m_size - pos;
                if (count < rlen) {
                    rlen = count;
                }

                if (rlen > 0) {
                    char* dest = data();
                    usize tail_len = m_size - (pos + rlen);
                    for (usize i = 0; i <= tail_len; ++i) {
                        dest[pos + i] = dest[pos + rlen + i];
                    }
                    m_size -= rlen;
                }
                return *this;
            }

            [[nodiscard]] usize find(char ch, usize pos = 0) const noexcept {
                if (pos >= m_size) {
                    return npos;
                }
                const char* base = data();
                for (usize i = pos; i < m_size; ++i) {
                    if (base[i] == ch) {
                        return i;
                    }
                }
                return npos;
            }

            [[nodiscard]] usize find(const char* str, usize pos = 0) const noexcept {
                if (!str) {
                    return npos;
                }
                usize str_len = c_str_len(str);
                if (str_len == 0) {
                    return (pos <= m_size) ? pos : npos;
                }
                if (str_len > m_size || pos > m_size - str_len) {
                    return npos;
                }

                const char* base = data();
                for (usize i = pos; i <= m_size - str_len; ++i) {
                    bool match = true;
                    for (usize j = 0; j < str_len; ++j) {
                        if (base[i + j] != str[j]) {
                            match = false;
                            break;
                        }
                    }
                    if (match) {
                        return i;
                    }
                }
                return npos;
            }

            template <typename OtherString>
            [[nodiscard]] usize find(const OtherString& other, usize pos = 0) const noexcept {
                return find(other.c_str(), pos);
            }

            __basic_string& insert(usize index, const char* str, usize count) {
                if (index > m_size) {
                    index = m_size;
                }
                if (!str || count == 0) {
                    return *this;
                }

                usize new_size = m_size + count;

                if (new_size > m_capacity) {
                    usize new_cap = m_capacity * 2;
                    if (new_cap < new_size) new_cap = new_size;

                    char* old_data = data();
                    char* new_data = static_cast<char*>(::operator new(new_cap + 1));

                    for (usize i = 0; i < index; ++i) {
                        new_data[i] = old_data[i];
                    }

                    for (usize i = 0; i < count; ++i) {
                        new_data[index + i] = str[i];
                    }
                    
                    for (usize i = index; i < m_size; ++i) {
                        new_data[count + i] = old_data[i];
                    }
                    new_data[new_size] = '\0';

                    free_heap();
                    m_heap_data = new_data;
                    m_capacity = new_cap;
                    m_is_heap = true;
                    m_size = new_size;
                } else {
                    char* dest = data();

                    if (m_size > index) {
                        for (usize i = 0; i < m_size - index; ++i) {
                            dest[new_size - 1 - i] = dest[m_size - 1 - i];
                        }
                    }
                    
                    for (usize i = 0; i < count; ++i) {
                        dest[index + i] = str[i];
                    }
                    m_size = new_size;
                    dest[m_size] = '\0';
                }

                return *this;
            }

            __basic_string& insert(usize index, const char* str) {
                return insert(index, str, c_str_len(str));
            }

            template <fennlib::types::uint OtherSSO>
            __basic_string& insert(usize index, const __basic_string<OtherSSO>& other) {
                return insert(index, other.c_str(), other.size());
            }

            __basic_string& insert(usize index, char ch, usize count = 1) {
                char temp_buf[2] = {ch, '\0'};
                if (count == 1) {
                    return insert(index, temp_buf, 1);
                }
                
                for (usize i = 0; i < count; ++i) {
                    insert(index + i, temp_buf, 1);
                }
                return *this;
            }

            __basic_string& operator+=(const char* str) {
                if (!str) return *this;
                usize extra_len = c_str_len(str);
                if (extra_len == 0) return *this;

                usize new_size = m_size + extra_len;
                if (new_size > m_capacity) {
                    usize new_cap = m_capacity * 2;
                    if (new_cap < new_size) new_cap = new_size;
                    
                    char* old_data = data();
                    char* new_data = static_cast<char*>(::operator new(new_cap + 1));
                    
                    for (usize i = 0; i < m_size; ++i) {
                        new_data[i] = old_data[i];
                    }
                    
                    free_heap();
                    m_heap_data = new_data;
                    m_capacity = new_cap;
                    m_is_heap = true;
                }

                char* dest = data();
                for (usize i = 0; i < extra_len; ++i) {
                    dest[m_size + i] = str[i];
                }
                dest[new_size] = '\0';
                m_size = new_size;

                return *this;
            }

            __basic_string& insert(const char* it, char ch) {
                usize index = static_cast<usize>(it - data());
                return insert(index, ch, 1);
            }

            __basic_string& push_back(char ch) {
                return insert(m_size, ch, 1);
            }

            template <fennlib::types::uint OtherSSO>
            __basic_string& operator+=(const __basic_string<OtherSSO>& other) {
                return *this += other.c_str();
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

        template <fennlib::types::uint SSO1, fennlib::types::uint SSO2>
        __basic_string<SSO1> operator+(const __basic_string<SSO1>& lhs, const __basic_string<SSO2>& rhs) {
            __basic_string<SSO1> result = lhs;
            result += rhs;
            return result;
        }

        template <fennlib::types::uint SSO>
        __basic_string<SSO> operator+(const char* lhs, const __basic_string<SSO>& rhs) {
            __basic_string<SSO> result(lhs);
            result += rhs;
            return result;
        }

        template <fennlib::types::uint SSO>
        __basic_string<SSO> operator+(const __basic_string<SSO>& lhs, const char* rhs) {
            __basic_string<SSO> result = lhs;
            result += rhs;
            return result;
        }

        template <fennlib::types::uint SSO1, fennlib::types::uint SSO2>
        bool operator==(const __basic_string<SSO1>& lhs, const __basic_string<SSO2>& rhs) {
            return string_equals(lhs.c_str(), rhs.c_str());
        }

        template <fennlib::types::uint SSO>
        bool operator==(const __basic_string<SSO>& lhs, const char* rhs) {
            return string_equals(lhs.c_str(), rhs);
        }

        template <fennlib::types::uint SSO>
        bool operator==(const char* lhs, const __basic_string<SSO>& rhs) {
            return string_equals(lhs, rhs.c_str());
        }
    }

    using string    = fennlib::internal::__basic_string<23>;
    using string8   = fennlib::internal::__basic_string<8>;
    using string16  = fennlib::internal::__basic_string<16>;
    using string32  = fennlib::internal::__basic_string<32>;
    using string64  = fennlib::internal::__basic_string<64>;
    using string128 = fennlib::internal::__basic_string<128>;
    using string256 = fennlib::internal::__basic_string<256>;
    using string512 = fennlib::internal::__basic_string<512>;
    using string1024 = fennlib::internal::__basic_string<1024>;
    using string2048 = fennlib::internal::__basic_string<2048>;
    using string4096 = fennlib::internal::__basic_string<4096>;
    using string8192 = fennlib::internal::__basic_string<8192>;
}

#endif