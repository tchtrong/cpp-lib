export module cpplib:concepts.equality_comparable;

import std;
import :concepts.boolean_testable;
import :concepts.convertible_to;
import :concepts.same_as;

namespace cpplib {
    template <typename T, typename U>
    concept weakly_equality_comparable_with =
        requires(const std::remove_reference_t<T>& ttt, const std::remove_reference_t<U>& uuu) {
            { ttt == uuu } -> boolean_testable;
            { ttt != uuu } -> boolean_testable;
            { uuu == ttt } -> boolean_testable;
            { uuu != ttt } -> boolean_testable;
        };

    export template <typename T>
    concept equality_comparable = weakly_equality_comparable_with<T, T>;

    template <typename T, typename U, typename C = std::common_reference_t<const T&, const U&>>
    concept comparison_common_type_with_impl =
        same_as<std::common_reference_t<const T&, const U&>,
                std::common_reference_t<const U&, const T&>>
        && requires {
               requires convertible_to<const T&, const C&> || convertible_to<T, const C&>;
               requires convertible_to<const U&, const C&> || convertible_to<U, const C&>;
           };

    template <typename T, typename U>
    concept comparison_common_type_with =
        comparison_common_type_with_impl<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;

    export template <typename T, typename U>
    concept equality_comparable_with =
        equality_comparable<T> && equality_comparable<U> && comparison_common_type_with<T, U>
        && equality_comparable<std::common_reference_t<const std::remove_reference_t<T>&,
                                                       const std::remove_reference_t<U>&>>
        && weakly_equality_comparable_with<T, U>;
} // namespace cpplib