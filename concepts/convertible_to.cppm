export module cpplib:concepts.convertible_to;

import std;

namespace cpplib {
    export template <typename From, typename To>
    concept implicitly_convertible_to = std::is_convertible_v<From, To>;

    export template <typename From, typename To>
    concept explicitly_convertible_to = requires { static_cast<To>(std::declval<From>()); };

    export template <typename From, typename To>
    concept convertible_to =
        implicitly_convertible_to<From, To> && explicitly_convertible_to<From, To>;

} // namespace cpplib