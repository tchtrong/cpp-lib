#ifndef CPPLIB_ARRAY_H_
#define CPPLIB_ARRAY_H_

#include "cpplib_compare.hpp"

#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace cpplib {
    template <typename T, std::size_t N>
    class array {
    public:
        using value_type             = T;
        using size_type              = std::size_t;
        using difference_type        = std::ptrdiff_t;
        using reference              = value_type&;
        using const_reference        = const value_type&;
        using pointer                = value_type*;
        using const_pointer          = const value_type*;
        using iterator               = value_type*;
        using const_iterator         = const value_type*;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        constexpr auto at(size_type pos) -> reference {
            if (pos >= N) {
                throw std::out_of_range("");
            }
            return m_data[pos];
        }

        constexpr auto at(size_type pos) const -> const_reference {
            if (pos >= N) {
                throw std::out_of_range("");
            }
            return m_data[pos];
        }

        constexpr auto operator[](size_type pos) noexcept -> reference {
            return m_data[pos];
        }

        constexpr auto operator[](size_type pos) const noexcept -> const_reference {
            return m_data[pos];
        }

        constexpr auto front() noexcept -> reference {
            return m_data[0];
        }

        constexpr auto front() const noexcept -> const_reference {
            return m_data[0];
        }

        constexpr auto back() noexcept -> reference {
            return m_data[N - 1];
        }

        constexpr auto back() const noexcept -> const_reference {
            return m_data[N - 1];
        }

        constexpr auto data() noexcept -> pointer {
            return static_cast<pointer>(m_data);
        }

        constexpr auto data() const noexcept -> const_pointer {
            return static_cast<const_pointer>(m_data);
        }

        constexpr auto begin() noexcept -> iterator {
            return iterator(data());
        }

        constexpr auto begin() const noexcept -> const_iterator {
            return const_iterator(data());
        }

        constexpr auto cbegin() const noexcept -> const_iterator {
            return const_iterator(data());
        }

        constexpr auto end() noexcept -> iterator {
            return iterator(data() + N);
        }

        constexpr auto end() const noexcept -> const_iterator {
            return const_iterator(data() + N);
        }

        constexpr auto cend() const noexcept -> const_iterator {
            return const_iterator(data() + N);
        }

        constexpr auto rbegin() noexcept -> reverse_iterator {
            return reverse_iterator(end());
        }

        constexpr auto rbegin() const noexcept -> const_reverse_iterator {
            return const_reverse_iterator(end());
        }

        constexpr auto crbegin() const noexcept -> const_reverse_iterator {
            return const_reverse_iterator(end());
        }

        constexpr auto rend() noexcept -> reverse_iterator {
            return reverse_iterator(begin());
        }

        constexpr auto rend() const noexcept -> const_reverse_iterator {
            return const_reverse_iterator(begin());
        }

        constexpr auto crend() const noexcept -> const_reverse_iterator {
            return const_reverse_iterator(begin());
        }

        [[nodiscard]] constexpr auto size() const noexcept -> size_type {
            return N;
        }

        constexpr void fill(const T& value) {
            std::fill_n(begin(), N, value);
        }

        constexpr void swap(array& other) noexcept(std::is_nothrow_swappable_v<T>) {
            std::swap_ranges(begin(), end(), other.begin());
        }

        T m_data[N]; // NOLINT
    };

    template <typename T, typename... U>
    array(T, U...) -> array<std::enable_if_t<(std::is_same_v<T, U> && ...), T>, 1 + sizeof...(U)>;

    template <class T, std::size_t N>
    auto operator==(const array<T, N>& lhs, const array<T, N>& rhs) -> bool {
        return std::equal(lhs.begin(), lhs.begin(), rhs.begin());
    }

    template <class T, std::size_t N>
    constexpr auto operator<=>(const array<T, N>& lhs,
                               const array<T, N>& rhs) -> synth_three_way_result<T> {
        for (size_t idx = 0; idx < N; ++idx) {
            auto cmp_res = synth_three_way(lhs[idx], rhs[idx]);
            if (cmp_res != 0) {
                return cmp_res;
            }
        }
        return std::strong_ordering::equal;
    }
} // namespace cpplib

#endif // CPPLIB_ARRAY_H_