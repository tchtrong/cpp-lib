module;

export module cpplib:array;

import std;
import :compare;

export namespace cpplib {
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

    private:
        template <typename Self>
        using iter = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>,
                                        const_iterator, iterator>;
        template <typename Self>
        using rev_iter = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>,
                                            const_reverse_iterator, reverse_iterator>;

    public:
        constexpr auto at(this auto&& self, size_type pos) -> decltype(auto) {
            if (pos >= N) {
                throw std::out_of_range("");
            }
            return std::forward_like<decltype(self)>(self.m_data[pos]);
        }

        constexpr auto operator[](this auto&& self, size_type pos) noexcept -> decltype(auto) {
            return std::forward_like<decltype(self)>(self.m_data[pos]);
        }

        constexpr auto front(this auto&& self) noexcept -> decltype(auto) {
            return std::forward_like<decltype(self)>(self.m_data[0]);
        }

        constexpr auto back(this auto&& self) noexcept -> decltype(auto) {
            return std::forward_like<decltype(self)>(self.m_data[N - 1]);
        }

        constexpr auto data(this auto&& self) noexcept -> auto {
            return std::forward_like<decltype(self)>(std::to_address(self.m_data));
        }

        constexpr auto begin(this auto&& self) noexcept -> auto {
            return iter<decltype(self)>(self.data());
        }

        constexpr auto cbegin() const noexcept -> auto {
            return begin();
        }

        constexpr auto end(this auto&& self) noexcept -> auto {
            return iter<decltype(self)>(self.data() + N);
        }

        constexpr auto cend() const noexcept -> auto {
            return end();
        }

        constexpr auto rbegin(this auto&& self) noexcept -> auto {
            return rev_iter<decltype(self)>(self.end());
        }

        constexpr auto crbegin() const noexcept -> auto {
            return rbegin();
        }

        constexpr auto rend(this auto&& self) noexcept -> auto {
            return rev_iter<decltype(self)>(self.begin());
        }

        constexpr auto crend() const noexcept -> auto {
            return rend();
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

    template <typename T, std::size_t N>
    constexpr auto operator<=>(const array<T, N>& lhs,
                               const array<T, N>& rhs) -> synth_three_way_result<T> {
        for (std::size_t idx = 0; idx < N; ++idx) {
            auto cmp_res = synth_three_way(lhs[idx], rhs[idx]);
            if (cmp_res != 0) {
                return cmp_res;
            }
        }
        return std::strong_ordering::equal;
    }
} // namespace cpplib
