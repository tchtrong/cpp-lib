export module cpplib:concepts.reference;

import std;
import :concepts.convertible_to;
import :concepts.same_as;
import :concepts.type_traits;

namespace cpplib {
    export template <typename T>
    concept can_reference = requires(T& var) { typename T; };

    export template <typename T>
    concept dereferenceable = requires {
        { *std::declval<T&>() } -> can_reference;
    };

    export template <typename T>
    concept is_reference = is_compound<T> && std::is_reference_v<T>;

    export template <typename T>
    concept is_lvalue_reference = is_reference<T> && std::is_lvalue_reference_v<T>;

    export template <typename T>
    concept is_rvalue_reference = is_reference<T> && std::is_rvalue_reference_v<T>;

    export template <typename T, typename U>
    concept common_reference_with =
        same_as<std::common_reference_t<T, U>, std::common_reference_t<U, T>>
        && convertible_to<T, std::common_reference_t<T, U>>
        && convertible_to<U, std::common_reference_t<T, U>>;

} // namespace cpplib