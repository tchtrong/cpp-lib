module;

export module cpplib:iterator.iterator_traits;

import std;
import :iterator.concepts;

template <typename Iter>
concept has_pointer_member = requires { typename Iter::pointer; };

template <typename Iter>
concept has_reference_member = requires { typename Iter::reference; };

template <typename Iter>
concept has_iterator_category_member = requires { typename Iter::iterator_category; };

template <typename Iter>
concept has_arrow_operator = !has_pointer_member<Iter> && cpplib::legacy_input_iterator<Iter>
                          && requires(const Iter& iter) { iter.operator->(); };

template <typename Iter>
concept has_incrementable_traits_difference_type =
    requires { typename std::incrementable_traits<Iter>::difference_type; };

template <typename Iter>
concept non_legacy_iterator = !cpplib::legacy_iterator<Iter>;

template <typename Iter>
struct iterator_difference_type {
    using type = void;
};

template <has_incrementable_traits_difference_type Iter>
struct iterator_difference_type<Iter> {
    using type = std::incrementable_traits<Iter>::difference_type;
};

template <typename Iter>
using iterator_difference_type_t = iterator_difference_type<Iter>::type;

template <typename Iter>
struct iterator_reference {
    using type = std::iter_reference_t<Iter>;
};

template <has_reference_member Iter>
struct iterator_reference<Iter> {
    using type = Iter::reference;
};

template <typename Iter>
using iterator_reference_t = iterator_reference<Iter>::type;

template <typename Iter>
struct iterator_pointer {
    using type = void;
};

template <has_pointer_member Iter>
struct iterator_pointer<Iter> {
    using type = Iter::pointer;
};

template <has_arrow_operator Iter>
struct iterator_pointer<Iter> {
    using type = decltype(std::declval<const Iter&>().operator->());
};

template <typename Iter>
using iterator_pointer_t = iterator_pointer<Iter>::type;

template <cpplib::legacy_input_iterator Iter>
struct legacy_iterator_iterator_category {
    using type = std::input_iterator_tag;
};

template <cpplib::legacy_forward_iterator Iter>
struct legacy_iterator_iterator_category<Iter> {
    using type = std::forward_iterator_tag;
};

template <cpplib::legacy_bidirectional_iterator Iter>
struct legacy_iterator_iterator_category<Iter> {
    using type = std::bidirectional_iterator_tag;
};

template <cpplib::legacy_random_access_iterator Iter>
struct legacy_iterator_iterator_category<Iter> {
    using type = std::random_access_iterator_tag;
};

template <typename Iter>
struct iterator_iterator_category : legacy_iterator_iterator_category<Iter> {};

template <has_iterator_category_member Iter>
struct iterator_iterator_category<Iter> {
    using type = Iter::iterator_category;
};

template <typename Iter>
using iterator_iterator_category_t = iterator_iterator_category<Iter>::type;

namespace cpplib {
    export template <typename>
    struct iterator_traits {};

    export template <non_legacy_iterator Iter>
    struct iterator_traits<Iter> {
        using value_type        = Iter::value_type;
        using reference         = Iter::reference;
        using pointer           = iterator_pointer_t<Iter>;
        using difference_type   = Iter::difference_type;
        using iterator_category = Iter::iterator_category;
    };

    export template <cpplib::legacy_iterator Iter>
    struct iterator_traits<Iter> {
        using value_type        = void;
        using reference         = void;
        using pointer           = void;
        using difference_type   = iterator_difference_type_t<Iter>;
        using iterator_category = std::output_iterator_tag;
    };

    export template <cpplib::legacy_input_iterator Iter>
    struct iterator_traits<Iter> {
        using value_type        = std::indirectly_readable_traits<Iter>::value_type;
        using reference         = iterator_reference_t<Iter>;
        using pointer           = iterator_pointer_t<Iter>;
        using difference_type   = std::incrementable_traits<Iter>::difference_type;
        using iterator_category = iterator_iterator_category_t<Iter>;
    };
} // namespace cpplib