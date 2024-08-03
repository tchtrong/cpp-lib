export module cpplib:concepts.constructible;

import std;
import :concepts.convertible_to;
import :concepts.destructible;

namespace cpplib {
    export template <typename T, typename... Args>
    concept constructible = std::is_constructible_v<T, Args...>;

    export template <typename T, typename... Args>
    concept trivially_constructible = std::is_trivially_constructible_v<T, Args...>;

    export template <typename T, typename... Args>
    concept nothrow_constructible = std::is_nothrow_constructible_v<T, Args...>;

    export template <typename T, typename... Args>
    concept constructible_from = destructible<T> && constructible<T, Args...>;

    export template <typename T, typename... Args>
    concept trivially_constructible_from = destructible<T> && trivially_constructible<T, Args...>;

    export template <typename T, typename... Args>
    concept nothrow_constructible_from = destructible<T> && nothrow_constructible<T, Args...>;

    export template <typename T>
    concept move_constructible = constructible_from<T, T> && convertible_to<T, T>;

    export template <typename T>
    concept copy_constructible =
        move_constructible<T> && constructible_from<T, T&> && convertible_to<T&, T>
        && constructible_from<const T, T> && convertible_to<T, const T>
        && constructible_from<const T&, T> && convertible_to<T, const T&>;

    export template <typename T>
    concept default_initializable =
        constructible_from<T> && requires { T{}; } && requires { ::new T; };
} // namespace cpplib