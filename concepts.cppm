module;

export module cpplib:concepts;

import std;

export namespace cpplib {
    template <typename T>
    concept referenceable = requires(T& var) { typename T; };

    template <typename T, typename U>
    concept same_as = std::is_same_v<T, U> && std::is_same_v<U, T>;

    template <typename T>
    concept destructible = std::is_nothrow_destructible_v<T>;

    template <typename T, typename... Args>
    concept constructible_from = destructible<T> && std::is_constructible_v<T, Args...>;

    template <typename From, typename To>
    concept convertible_to =
        std::is_convertible_v<From, To> && requires { static_cast<To>(std::declval<From>()); };

    template <typename T>
    concept move_constructible = constructible_from<T, T> && convertible_to<T, T>;

    template <typename T>
    concept copy_constructible =
        move_constructible<T> && constructible_from<T, T&> && convertible_to<T&, T>
        && constructible_from<const T, T> && convertible_to<T, const T>
        && constructible_from<const T&, T> && convertible_to<T, const T&>;

    template <typename Derived, typename Base>
    concept derived_from = std::is_base_of_v<Base, Derived>
                        && std::is_convertible_v<const volatile Derived*, const volatile Base*>;

    template <typename B>
    concept boolean_testable_impl = convertible_to<B, bool>;

    template <typename B>
    concept boolean_testable = boolean_testable_impl<B> && requires(B&& b_b) {
        { !std::forward<B>(b_b) } -> boolean_testable_impl;
    };

} // namespace cpplib
