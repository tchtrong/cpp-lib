module;

export module cpplib:concepts;

import std;

namespace cpplib {
    export template <typename T>
    concept referenceable = requires(T& var) { typename T; };

    export template <typename T>
    concept dereferenceable = requires(T& var) {
        { *var } -> referenceable;
    };

    export template <typename T, typename U>
    concept same_as = std::is_same_v<T, U> && std::is_same_v<U, T>;

    export template <typename T>
    concept destructible = std::is_nothrow_destructible_v<T>;

    export template <typename T, typename... Args>
    concept constructible_from = destructible<T> && std::is_constructible_v<T, Args...>;

    export template <typename From, typename To>
    concept convertible_to =
        std::is_convertible_v<From, To> && requires { static_cast<To>(std::declval<From>()); };

    export template <typename T>
    concept move_constructible = constructible_from<T, T> && convertible_to<T, T>;

    export template <typename T>
    concept copy_constructible =
        move_constructible<T> && constructible_from<T, T&> && convertible_to<T&, T>
        && constructible_from<const T, T> && convertible_to<T, const T>
        && constructible_from<const T&, T> && convertible_to<T, const T&>;

    export template <typename T, typename U>
    concept common_reference_with =
        same_as<std::common_reference_t<T, U>, std::common_reference_t<U, T>>
        && convertible_to<T, std::common_reference_t<T, U>>
        && convertible_to<U, std::common_reference_t<T, U>>;

    export template <typename LHS, typename RHS>
    concept assignable_from = std::is_lvalue_reference_v<LHS>
                           && common_reference_with<const std::remove_reference_t<LHS>&,
                                                    const std::remove_reference_t<RHS>&>
                           && requires(LHS lhs, RHS&& rhs) {
                                  { lhs = std::forward<RHS>(rhs) } -> same_as<LHS>;
                              };

    export template <typename T>
    concept swappable = requires(T& lhs, T& rhs) { std::ranges::swap(lhs, rhs); };

    export template <typename T, typename U>
    concept swappable_with = common_reference_with<T, U> && requires(T&& first, U&& second) {
        std::ranges::swap(std::forward<T>(first), std::forward<T>(first));
        std::ranges::swap(std::forward<U>(second), std::forward<U>(second));
        std::ranges::swap(std::forward<T>(first), std::forward<U>(second));
        std::ranges::swap(std::forward<U>(second), std::forward<T>(first));
    };

    export template <typename T>
    concept movable =
        std::is_object_v<T> && move_constructible<T> && assignable_from<T&, T> && swappable<T>;

    export template <typename T>
    concept copyable = copy_constructible<T> && movable<T> && assignable_from<T&, T&>
                    && assignable_from<T&, const T&> && assignable_from<T&, const T>;

    export template <typename Derived, typename Base>
    concept derived_from = std::is_base_of_v<Base, Derived>
                        && std::is_convertible_v<const volatile Derived*, const volatile Base*>;

    template <typename B>
    concept boolean_testable_impl = convertible_to<B, bool>;

    export template <typename B>
    concept boolean_testable = boolean_testable_impl<B> && requires(B&& b_b) {
        { !std::forward<B>(b_b) } -> boolean_testable_impl;
    };
} // namespace cpplib
