#if !defined(__FENNLIB_ALLOW_INTERNAL__) && !defined(__clangd__) && !defined(__INTELLISENSE__) && !defined(__clang_analyzer__)
    #error "fennlib/cxx17/_sequence.hpp is an internal header! Include <sequence> or <fennlib> instead."
#endif

#if !defined(__FENNLIB_INTERNAL_CXX17_SEQUENCE_HPP) && defined(__FENNLIB_ALLOW_INTERNAL__)
#define __FENNLIB_INTERNAL_CXX17_SEQUENCE_HPP

#include "_types.hpp"
#include "_is_same.hpp"

namespace fennlib {
    struct default_allocator {
        template <typename T>
        [[nodiscard]] static T* allocate(usize count) {
            if (count == 0) return nullptr;
            return static_cast<T*>(::operator new(count * sizeof(T)));
        }

        template <typename T>
        static void deallocate(T* ptr, usize count) noexcept {
            (void)count;
            ::operator delete(ptr);
        }
    };

    template <typename T, typename Allocator = default_allocator>
    class sequence {
    private:
        T* m_data{nullptr};
        usize m_capacity{0};
        usize m_size{0};
        static constexpr usize ahead = 10u;

        void reallocate(usize new_capacity) {
            T* new_block = Allocator::template allocate<T>(new_capacity);

            for (usize i = 0; i < m_size; ++i) {
                new_block[i] = static_cast<T&&>(m_data[i]);
                m_data[i].~T(); 
            }

            if (m_data) {
                Allocator::template deallocate<T>(m_data, m_capacity);
            }

            m_data = new_block;
            m_capacity = new_capacity;
        }

        void ensure_capacity(usize required_capacity) {
            if (required_capacity > m_capacity) {
                usize new_cap = m_capacity == 0 ? ahead : m_capacity * 2;
                while (new_cap < required_capacity) {
                    new_cap *= 2;
                }
                reallocate(new_cap);
            }
        }

    public:
        constexpr sequence() noexcept = default;

        template <typename... Args>
        constexpr sequence(Args&&... args) {
            constexpr usize arg_count = sizeof...(Args);
            if constexpr (arg_count > 0) {
                m_capacity = (arg_count + ahead > arg_count * 2) 
                             ? (arg_count + ahead) 
                             : (arg_count * 2);

                m_data = Allocator::template allocate<T>(m_capacity);

                ((m_data[m_size++] = static_cast<T>(args)), ...);
            }
        }

        
        template <typename Dummy = void>
        constexpr sequence(const char* str) {
            static_assert(fennlib::is_same_v<T, char>, "fennlib::sequence C-string constructor is only valid for T = char");
            if (str) {
                usize len = 0;
                while (str[len] != '\0') {
                    ++len;
                }
                ensure_capacity(len);
                for (usize i = 0; i < len; ++i) {
                    m_data[i] = str[i];
                }
                m_size = len;
            }
        }

        ~sequence() noexcept {
            clear();
            if (m_data) {
                Allocator::template deallocate<T>(m_data, m_capacity);
            }
        }

        void clear() noexcept {
            for (usize i = 0; i < m_size; ++i) {
                m_data[i].~T();
            }
            m_size = 0;
        }

        void push_back(const T& value) {
            ensure_capacity(m_size + 1);
            m_data[m_size++] = value;
        }

        void push_back(T&& value) {
            ensure_capacity(m_size + 1);
            m_data[m_size++] = static_cast<T&&>(value);
        }

        template <typename First, typename Second, typename... Rest>
        void push_back(First&& first, Second&& second, Rest&&... rest) {
            constexpr usize total_new = 2 + sizeof...(Rest);
            ensure_capacity(m_size + total_new);
            
            m_data[m_size++] = static_cast<First&&>(first);
            m_data[m_size++] = static_cast<Second&&>(second);
            ((m_data[m_size++] = static_cast<Rest&&>(rest)), ...);
        }

        void pop_back(usize n = 1) noexcept {
            if (n > m_size) {
                n = m_size;
            }
            for (usize i = 0; i < n; ++i) {
                --m_size;
                m_data[m_size].~T();
            }
        }

        [[nodiscard]] constexpr usize size() const noexcept { return m_size; }
        [[nodiscard]] constexpr usize capacity() const noexcept { return m_capacity; }
        [[nodiscard]] constexpr bool empty() const noexcept { return m_size == 0; }

        [[nodiscard]] constexpr T* data() noexcept { return m_data; }
        [[nodiscard]] constexpr const T* data() const noexcept { return m_data; }

        [[nodiscard]] constexpr T& operator[](usize index) noexcept { return m_data[index]; }
        [[nodiscard]] constexpr const T& operator[](usize index) const noexcept { return m_data[index]; }

        [[nodiscard]] constexpr T* begin() noexcept { return m_data; }
        [[nodiscard]] constexpr const T* begin() const noexcept { return m_data; }
        [[nodiscard]] constexpr T* end() noexcept { return m_data + m_size; }
        [[nodiscard]] constexpr const T* end() const noexcept { return m_data + m_size; }
    };
}

#endif