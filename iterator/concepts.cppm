export module cpplib:iterator.concepts;

import std;
import :concepts;

namespace cpplib {
    export template <typename I>
    concept legacy_iterator = requires(I iter) {
        { *iter } -> referenceable;
        { ++iter } -> same_as<I&>;
        { *iter++ } -> referenceable;
    } && copyable<I>;

    export template <class I>
    concept legacy_input_iterator =
        legacy_iterator<I> && std::equality_comparable<I> && requires(I iter) {
            typename std::incrementable_traits<I>::difference_type;
            requires std::signed_integral<typename std::incrementable_traits<I>::difference_type>;
            typename std::indirectly_readable_traits<I>::value_type;
            typename std::common_reference_t<
                std::iter_reference_t<I>&&,
                typename std::indirectly_readable_traits<I>::value_type&>;
            typename std::common_reference_t<
                decltype(*iter++)&&, typename std::indirectly_readable_traits<I>::value_type&>;
        };

    export template <class I>
    concept legacy_forward_iterator =
        legacy_input_iterator<I> && constructible_from<I>
        && std::is_reference_v<std::iter_reference_t<I>>
        && same_as<std::remove_cvref_t<std::iter_reference_t<I>>,
                   typename std::indirectly_readable_traits<I>::value_type>
        && requires(I iter) {
               { iter++ } -> convertible_to<const I&>;
               { *iter++ } -> same_as<std::iter_reference_t<I>>;
           };

    export template <class I>
    concept legacy_bidirectional_iterator = legacy_forward_iterator<I> && requires(I iter) {
        { --iter } -> same_as<I&>;
        { iter-- } -> convertible_to<const I&>;
        { *iter-- } -> same_as<std::iter_reference_t<I>>;
    };

    export template <class I>
    concept legacy_random_access_iterator =
        legacy_bidirectional_iterator<I> && std::totally_ordered<I>
        && requires(I iter, typename std::incrementable_traits<I>::difference_type dist) {
               { iter += dist } -> same_as<I&>;
               { iter -= dist } -> same_as<I&>;
               { iter + dist } -> same_as<I>;
               { dist + iter } -> same_as<I>;
               { iter - dist } -> same_as<I>;
               { iter - iter } -> same_as<decltype(dist)>;
               { iter[dist] } -> convertible_to<std::iter_reference_t<I>>;
           };
} // namespace cpplib
