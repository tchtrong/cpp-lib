module;

export module cpplib:doubly_list;

import std;
import :array;
import :compare;
import :concepts;
import :iterator;

struct node_base {
    constexpr node_base() noexcept = default;

    constexpr node_base(node_base&& other) noexcept : node_base() {
        *this = std::move(other);
    }

    constexpr auto operator=(node_base&& other) noexcept -> node_base& {
        if (other.mp_next != &other) {
            connect_after(other.mp_prev);
            other.detach().reset();
        }
        return *this;
    };

    constexpr node_base(const node_base& other) noexcept = delete;

    constexpr auto operator=(const node_base& other) noexcept -> node_base& = delete;

    constexpr ~node_base() = default;

    constexpr auto reset() noexcept -> node_base& {
        mp_next = mp_prev = this;
        return *this;
    }

    constexpr auto connect_after(node_base* nodeptr) noexcept -> node_base& {
        mp_next          = nodeptr->mp_next;
        mp_prev          = nodeptr;
        nodeptr->mp_next = this;
        mp_next->mp_prev = this;
        return *this;
    }

    constexpr auto connect_sequence(node_base* first, node_base* last) noexcept -> node_base& {
        first->mp_prev->mp_next = last->mp_next;
        last->mp_next->mp_prev  = first->mp_prev;
        last->mp_next           = mp_next;
        mp_next->mp_prev        = last;
        first->mp_prev          = this;
        mp_next                 = first;
        return *this;
    }

    constexpr auto detach() noexcept -> node_base& {
        if (mp_next == mp_prev && mp_prev == this) {
            return *this;
        }
        mp_next->mp_prev = mp_prev;
        mp_prev->mp_next = mp_next;
        return *this;
    }

    constexpr void swap(node_base& other) noexcept {
        auto* tmp              = other.mp_prev;
        other.mp_prev          = mp_prev;
        other.mp_prev->mp_next = &other;
        mp_prev                = tmp;
        mp_prev->mp_next       = this;

        tmp                    = other.mp_next;
        other.mp_next          = mp_next;
        other.mp_next->mp_prev = &other;
        mp_next                = tmp;
        mp_next->mp_prev       = this;
    }

    node_base* mp_prev{this};
    node_base* mp_next{this};
};

export namespace cpplib {
    template <typename T, typename Allocator = std::allocator<T>>
        requires(same_as<std::remove_cv_t<T>, T> && same_as<typename Allocator::value_type, T>)
    class doubly_list {
    public:
        using value_type     = T;
        using allocator_type = Allocator;

    private:
        using list_type        = doubly_list<value_type, allocator_type>;
        using allocator_traits = std::allocator_traits<allocator_type>;

    public:
        using reference       = value_type&;
        using const_reference = const value_type&;
        using pointer         = allocator_traits::pointer;
        using const_pointer   = allocator_traits::const_pointer;
        using difference_type = allocator_traits::difference_type;
        using size_type       = allocator_traits::size_type;

    private:
        struct node : public node_base {
            constexpr auto get_data_ptr() const noexcept -> pointer {
                return std::bit_cast<pointer>(std::addressof(m_data));
            }

            constexpr auto get_data() const noexcept -> reference {
                return *get_data_ptr();
            }

            alignas(value_type) array<std::byte, sizeof(value_type)> m_data;
        };

        using node_allocator_type   = allocator_traits::template rebind_alloc<node>;
        using node_allocator_traits = allocator_traits::template rebind_traits<node>;
        using void_pointer          = allocator_traits::void_pointer;
        using void_pointer_traits   = std::pointer_traits<void_pointer>;

        template <typename IT>
        class iterator_impl {
        public:
            using value_type        = IT;
            using reference         = value_type&;
            using pointer           = void_pointer_traits::template rebind<value_type>;
            using difference_type   = std::pointer_traits<pointer>::difference_type;
            using iterator_category = std::bidirectional_iterator_tag;

            friend list_type;

            constexpr iterator_impl() noexcept = default;

            // NOLINTBEGIN: Allow for implicit conversion from iterator to
            // const_iterator
            template <is_const U = IT>
            constexpr iterator_impl(const iterator_impl<std::remove_const_t<U>>& other) noexcept
                : iterator_impl(other.mp_node){};
            // NOLINTEND

            constexpr auto operator*() const noexcept -> decltype(auto) {
                return static_cast<node*>(mp_node)->get_data();
            }

            constexpr auto operator->() const noexcept -> decltype(auto) {
                return static_cast<node*>(mp_node)->get_data_ptr();
            }

            constexpr auto operator++() noexcept -> iterator_impl& {
                mp_node = mp_node->mp_next;
                return *this;
            }

            constexpr auto operator++(int) noexcept -> iterator_impl {
                auto res = *this;
                ++(*this);
                return res;
            }

            constexpr auto operator--() noexcept -> iterator_impl& {
                mp_node = mp_node->mp_prev;
                return *this;
            }

            constexpr auto operator--(int) noexcept -> iterator_impl {
                auto res = *this;
                --(*this);
                return res;
            }

            constexpr auto operator==(const iterator_impl& other) const noexcept -> bool = default;

            template <typename U = IT, typename = std::enable_if_t<!std::is_const_v<U>>>
            constexpr auto operator==(const iterator_impl<const U>& other) const noexcept -> bool {
                return this->mp_node == other.mp_node;
            };

        private:
            constexpr explicit iterator_impl(node_base* p_node) noexcept : mp_node(p_node) {};

            node_base* mp_node{};
        };

    public:
        using iterator               = iterator_impl<value_type>;
        using const_iterator         = iterator_impl<const value_type>;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        static_assert(legacy_bidirectional_iterator<iterator>);
        static_assert(legacy_bidirectional_iterator<const_iterator>);
        static_assert(legacy_bidirectional_iterator<reverse_iterator>);
        static_assert(legacy_bidirectional_iterator<const_reverse_iterator>);
        static_assert(convertible_to<iterator, const_iterator>);
        static_assert(not convertible_to<const_iterator, iterator>);
        static_assert(equality_comparable_with<iterator, const_iterator>);

    private:
        template <typename Self>
        using iter = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>,
                                        const_iterator, iterator>;
        template <typename Self>
        using reverse_iter = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>,
                                                const_reverse_iterator, reverse_iterator>;

    public:
        constexpr doubly_list() noexcept = default;

        explicit constexpr doubly_list(const allocator_type& alloc) noexcept
            : m_node_allocator(alloc) {}

        constexpr doubly_list(size_type count, const T& value,
                              const allocator_type& alloc = allocator_type())
            noexcept(noexcept(insert(end(), count, value)))
            : doubly_list(alloc) {
            insert(end(), count, value);
        }

        explicit constexpr doubly_list(size_type             count,
                                       const allocator_type& alloc = allocator_type())
            noexcept(noexcept(emplace(end())))
            : doubly_list(alloc) {
            for (size_type idx = 0; idx < count; ++idx) {
                emplace(end());
            }
        }

        template <std::forward_iterator InputIt>
        constexpr doubly_list(InputIt first, InputIt last,
                              const allocator_type& alloc = allocator_type())
            noexcept(noexcept(insert(end(), first, last)))
            : doubly_list(alloc) {
            insert(end(), first, last);
        }

        constexpr doubly_list(const doubly_list& other)
            noexcept(noexcept(insert(end(), other.begin(), other.end())))
            : doubly_list(node_allocator_traits::select_on_container_copy_construction(
                  other.m_node_allocator)) {
            insert(end(), other.begin(), other.end());
        }

        constexpr doubly_list(const doubly_list& other, const allocator_type& alloc)
            noexcept(noexcept(insert(end(), other.begin(), other.end())))
            : doubly_list(alloc) {
            insert(end(), other.begin(), other.end());
        }

        constexpr doubly_list(doubly_list&& other) noexcept
            : m_node_allocator(std::move(other.m_node_allocator)), m_end(std::move(other.m_end)),
              m_size(other.m_size) {
            other.m_size = 0;
        };

        constexpr doubly_list(doubly_list&& other, const allocator_type& alloc)
            noexcept(node_allocator_traits::is_always_equal::value
                     || noexcept(insert(end(), std::make_move_iterator(other.begin()),
                                        std::make_move_iterator(other.end()))))
            : doubly_list(alloc) {
            if constexpr (node_allocator_traits::is_always_equal::value) {
                m_end        = std::move(other.m_end);
                m_size       = other.m_size;
                other.m_size = 0;
            } else {
                if (m_node_allocator == other.m_node_allocator) {
                    m_end        = std::move(other.m_end);
                    m_size       = other.m_size;
                    other.m_size = 0;
                } else {
                    insert(end(), std::make_move_iterator(other.begin()),
                           std::make_move_iterator(other.end()));
                }
            }
        }

        template <size_type N>
        explicit constexpr doubly_list(array<value_type, N>&& in_array,
                                       const allocator_type&  alloc = std::allocator<value_type>())
            noexcept(noexcept(doubly_list(std::make_move_iterator(in_array.begin()),
                                          std::make_move_iterator(in_array.end()), alloc)))
            : doubly_list(std::make_move_iterator(in_array.begin()),
                          std::make_move_iterator(in_array.end()), alloc) {}

        constexpr ~doubly_list() noexcept(noexcept(clear())) {
            clear();
        }

        constexpr auto operator=(const doubly_list& other)
            noexcept(noexcept(assign_dispatch(other.begin(), other.end()))
                     && (!node_allocator_traits::propagate_on_container_copy_assignment::value
                         || node_allocator_traits::is_always_equal::value
                         || noexcept(clear()))) -> doubly_list& {
            if (this == &other) {
                return *this;
            }
            if constexpr (node_allocator_traits::propagate_on_container_copy_assignment::value) {
                if constexpr (!node_allocator_traits::is_always_equal::value) {
                    if (m_node_allocator != other.m_node_allocator) {
                        clear();
                    }
                }
                m_node_allocator = other.m_node_allocator;
            }
            assign_dispatch(other.begin(), other.end());
            return *this;
        }

        constexpr auto operator=(doubly_list&& other)
            noexcept(noexcept(move_assign(other))
                     && (node_allocator_traits::is_always_equal::value
                         || node_allocator_traits::propagate_on_container_move_assignment::value
                         || noexcept(assign_dispatch(std::make_move_iterator(other.begin()),
                                                     std::make_move_iterator(other.end())))))
                -> doubly_list& {
            if constexpr (node_allocator_traits::is_always_equal::value
                          || node_allocator_traits::propagate_on_container_move_assignment::value) {
                move_assign(other);
            } else {
                if (m_node_allocator == other.m_node_allocator) {
                    move_assign(other);
                } else {
                    assign_dispatch(std::make_move_iterator(other.begin()),
                                    std::make_move_iterator(other.end()));
                }
            }
            return *this;
        }

        [[nodiscard]] constexpr auto get_allocator() const noexcept -> allocator_type {
            return m_node_allocator;
        }

        [[nodiscard]] constexpr auto front(this auto&& self) noexcept -> auto&& {
            return std::forward_like<decltype(self)>(
                static_cast<node*>(self.m_end.mp_next)->get_data());
        }

        [[nodiscard]] constexpr auto back(this auto&& self) noexcept -> auto&& {
            return std::forward_like<decltype(self)>(
                static_cast<node*>(self.m_end.mp_prev)->get_data());
        }

        [[nodiscard]] constexpr auto begin(this auto&& self) noexcept {
            return iter<decltype(self)>(self.m_end.mp_next);
        }

        [[nodiscard]] constexpr auto cbegin() const noexcept -> auto {
            return begin();
        }

        [[nodiscard]] constexpr auto end(this auto&& self) noexcept {
            return iter<decltype(self)>(const_cast<node_base*>(&self.m_end));
        }

        [[nodiscard]] constexpr auto cend() const noexcept -> auto {
            return end();
        }

        [[nodiscard]] constexpr auto empty() const noexcept -> bool {
            return m_size == 0;
        }

        [[nodiscard]] constexpr auto size() const noexcept -> size_type {
            return m_size;
        }

        template <class... Args>
        constexpr auto emplace(const_iterator pos, Args&&... args)
            noexcept(noexcept(construct_node(std::forward<Args>(args)...))) -> iterator {
            node* p_new = construct_node(std::forward<Args>(args)...);
            p_new->connect_after(pos.mp_node->mp_prev);
            ++m_size;
            return iterator(p_new);
        }

        constexpr auto insert(const_iterator pos, const T& value)
            noexcept(noexcept(emplace(pos, value))) -> iterator {
            return emplace(pos, value);
        }

        constexpr auto insert(const_iterator pos, T&& value)
            noexcept(noexcept(emplace(pos, std::move(value)))) -> iterator {
            return emplace(pos, std::move(value));
        }

        constexpr auto insert(const_iterator pos, size_type count, const T& value)
            noexcept(noexcept(emplace(pos, value))) -> iterator {
            if (count == 0) {
                return pos;
            }
            iterator res = emplace(pos, value);
            for (size_type idx = 0; idx < count - 1; ++idx) {
                emplace(pos, value);
            }
            return res;
        }

        template <cpplib::legacy_input_iterator InputIt>
        constexpr auto insert(const_iterator pos, InputIt first, InputIt last)
            noexcept(noexcept(emplace(pos, *first))) -> iterator {
            if (first == last) {
                return iterator(pos.mp_node);
            }
            iterator res = emplace(pos, *first);
            ++first;
            while (first != last) {
                emplace(pos, *first);
                ++first;
            }
            return res;
        }

        constexpr auto erase(const_iterator pos)
            noexcept(noexcept(destruct_node(std::declval<node*>()))) -> iterator {
            if (pos.mp_node == &m_end) {
                return iterator(pos.mp_node);
            }
            auto*      next_node = pos.mp_node->mp_next;
            node_base* cur_node  = &(pos.mp_node->detach());
            destruct_node(static_cast<node*>(cur_node));
            --m_size;
            return iterator(next_node);
        }

        constexpr auto erase(const_iterator first, const_iterator last)
            noexcept(noexcept(erase(first))) -> iterator {
            while (first != last) {
                erase(first++);
            }
            return iterator(last.mp_node);
        }

        constexpr void clear() noexcept(noexcept(erase(begin(), end()))) {
            erase(begin(), end());
        }

        constexpr void push_back(const T& value) noexcept(noexcept(emplace(end(), value))) {
            emplace(end(), value);
        }

        constexpr void push_back(T&& value) noexcept(noexcept(emplace(end(), std::move(value)))) {
            emplace(end(), std::move(value));
        }

        template <class... Args>
        constexpr auto emplace_back(Args&&... args)
            noexcept(noexcept(emplace(end(), std::forward<Args>(args)...))) -> reference {
            return *(emplace(end(), std::forward<Args>(args)...));
        }

        constexpr void pop_back() noexcept(noexcept(erase(--end()))) {
            erase(--end());
        }

        constexpr void push_front(const T& value) noexcept(noexcept(emplace(begin(), value))) {
            emplace(begin(), value);
        }

        constexpr void push_front(T&& value)
            noexcept(noexcept(emplace(begin(), std::move(value)))) {
            emplace(begin(), std::move(value));
        }

        template <class... Args>
        constexpr auto emplace_front(Args&&... args)
            noexcept(noexcept(emplace(begin(), std::forward<Args>(args)...))) -> reference {
            return *(emplace(begin(), std::forward<Args>(args)...));
        }

        constexpr void pop_front() noexcept(noexcept(erase(begin()))) {
            erase(begin());
        }

        constexpr void resize(size_type count) {
            if (count == m_size) {
                return;
            }
            if (count > m_size) {
                for (size_type idx = 0; idx < count - m_size; ++idx) {
                    emplace(end());
                }
            } else {
                for (size_type idx = 0; idx < m_size - count; ++idx) {
                    pop_back();
                }
            }
        }

        constexpr void resize(size_type count, const value_type& value) {
            if (count == m_size) {
                return;
            }
            if (count > m_size) {
                for (size_type idx = 0; idx < count - m_size; ++idx) {
                    emplace(end(), value);
                }
            } else {
                for (size_type idx = 0; idx < m_size - count; ++idx) {
                    pop_back();
                }
            }
        }

        constexpr void swap(doubly_list& other) noexcept {
            if constexpr (!node_allocator_traits::is_always_equal::value) {
                if (m_node_allocator != other.m_node_allocator) {
                    return;
                }
            }
            if constexpr (node_allocator_traits::propogate_on_containter_swap::value) {
                std::swap(m_node_allocator, other.m_node_allocator);
            }
            m_end.swap(other.m_end);
            std::swap(m_size, other.m_size);
        }

        constexpr void merge(doubly_list& other) noexcept {
            merge(std::move(other));
        }

        constexpr void merge(doubly_list&& other) noexcept {
            merge(std::move(other), std::less<>{});
        }

        template <typename Compare>
        constexpr void merge(doubly_list& other, Compare comp) noexcept {
            merge(std::move(other), comp);
        }

        template <typename Compare>
        constexpr void merge(doubly_list&& other, Compare comp) noexcept {
            if (this == &other) {
                return;
            }

            if constexpr (!node_allocator_traits::is_always_equal::value) {
                if (m_node_allocator != other.m_node_allocator) {
                    return;
                }
            }

            m_size += other.m_size;
            other.m_size = 0;

            auto first1 = begin();
            auto last1  = end();
            auto first2 = other.begin();
            auto last2  = other.end();

            while (first1 != last1 && first2 != last2) {
                if (comp(*first2, *first1)) {
                    auto next = first2;
                    ++next;
                    first2.mp_node->detach().connect_after(first1.mp_node->mp_prev);
                    first2 = next;
                } else {
                    ++first1;
                }
            }

            if (first2 != last2) {
                m_end.mp_prev->connect_sequence(first2.mp_node, last2.mp_node->mp_prev);
            }
        }

        constexpr void sort() noexcept(noexcept(sort(std::less<>{}))) {
            sort(std::less<>{});
        }

        template <class Compare>
        constexpr void sort(Compare comp);

    private:
        constexpr void destruct_node(node* nodeptr)
            noexcept(noexcept(node_allocator_traits::destroy(m_node_allocator,
                                                             std::declval<pointer>()))) {
            node_allocator_traits::destroy(m_node_allocator, nodeptr->get_data_ptr());
            node_allocator_traits::destroy(m_node_allocator, nodeptr);
            m_node_allocator.deallocate(nodeptr, 1);
        }

        template <typename... Args>
        constexpr auto construct_node(Args&&... args) noexcept(
            noexcept(m_node_allocator.allocate(1))
            && noexcept(node_allocator_traits::construct(m_node_allocator, std::declval<pointer>(),
                                                         std::forward<Args>(args)...))) -> node* {
            node* p_new = m_node_allocator.allocate(1);
            node_allocator_traits::construct(m_node_allocator, p_new);
            node_allocator_traits::construct(m_node_allocator, p_new->get_data_ptr(),
                                             std::forward<Args>(args)...);
            return p_new;
        }

        template <cpplib::legacy_input_iterator InputIt>
        constexpr void assign_dispatch(InputIt first, InputIt last)
            noexcept(noexcept(*begin() = *first) && noexcept(erase(begin(), end()))
                     && noexcept(insert(begin(), first, last))) {
            auto this_iter = begin();
            auto this_last = end();
            for (; this_iter != this_last && first != last; ++this_iter, ++first) {
                *this_iter = *first;
            }
            if (first == last) {
                erase(this_iter, this_last);
            }
            insert(this_last, first, last);
        }

        constexpr void move_assign(doubly_list& other) noexcept(noexcept(clear())) {
            clear();
            if constexpr (node_allocator_traits::propagate_on_container_move_assignment::value) {
                m_node_allocator = std::move(other.m_node_allocator);
            }
            m_end        = std::move(other.m_end);
            m_size       = other.m_size;
            other.m_size = 0;
        }

        node_allocator_type m_node_allocator{};
        node_base           m_end;
        size_type           m_size{};
    };

    template <class TT, class Alloc>
    constexpr auto operator==(const doubly_list<TT, Alloc>& lhs,
                              const doubly_list<TT, Alloc>& rhs) noexcept -> bool {
        return lhs.size() == rhs.size()
            && std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <class TT, class Alloc>
    constexpr auto operator<=>(const doubly_list<TT, Alloc>& lhs,
                               const doubly_list<TT, Alloc>& rhs) -> synth_three_way_result<TT> {
        return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(),
                                                      rhs.end(), synth_three_way);
    }
} // namespace cpplib
