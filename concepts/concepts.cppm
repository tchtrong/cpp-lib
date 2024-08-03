module;

export module cpplib:concepts;

export import :concepts.arithmetic;
export import :concepts.boolean_testable;
export import :concepts.constructible;
export import :concepts.convertible_to;
export import :concepts.destructible;
export import :concepts.equality_comparable;
export import :concepts.reference;
export import :concepts.same_as;
export import :concepts.swappable;
export import :concepts.type_traits;

import std;

namespace cpplib {
    export template <typename T, typename U>
    concept common_with = std::same_as<std::common_type_t<T, U>, std::common_type_t<U, T>>
                       && explicitly_convertible_to<T, std::common_type_t<T, U>>
                       && explicitly_convertible_to<U, std::common_type_t<T, U>>
                       && std::common_reference_with<std::add_lvalue_reference_t<const T>,
                                                     std::add_lvalue_reference_t<const U>>
                       && std::common_reference_with<
                              std::add_lvalue_reference_t<std::common_type_t<T, U>>,
                              std::common_reference_t<std::add_lvalue_reference_t<const T>,
                                                      std::add_lvalue_reference_t<const U>>>;

    export template <typename LHS, typename RHS>
    concept assignable_from = lvalue_reference<LHS>
                           && common_reference_with<const std::remove_reference_t<LHS>&,
                                                    const std::remove_reference_t<RHS>&>
                           && requires(LHS lhs, RHS&& rhs) {
                                  { lhs = std::forward<RHS>(rhs) } -> same_as<LHS>;
                              };

    export template <typename T>
    concept movable = object<T> && move_constructible<T> && assignable_from<T&, T> && swappable<T>;

    export template <typename T>
    concept copyable = copy_constructible<T> && movable<T> && assignable_from<T&, T&>
                    && assignable_from<T&, const T&> && assignable_from<T&, const T>;

    export template <typename Derived, typename Base>
    concept derived_from = base_of<Base, Derived>
                        && implicitly_convertible_to<const volatile Derived*, const volatile Base*>;
} // namespace cpplib
