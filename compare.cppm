module;

export module cpplib:compare;

import std;
import :concepts;

export namespace cpplib {
    constexpr auto synth_three_way =
        []<typename T, typename U>(const T& t_t, const U& u_u) noexcept(
            (std::three_way_comparable_with<T, U>&& noexcept(std::declval<T&>()
                                                             <=> std::declval<U&>()))
            || (noexcept(std::declval<T&>() < std::declval<U&>())&& noexcept(std::declval<U&>()
                                                                             < std::declval<T&>())))
        requires requires {
            { t_t < u_u } -> boolean_testable;
            { u_u < t_t } -> boolean_testable;
        }
    {
        if constexpr (std::three_way_comparable_with<T, U>) {
            return t_t <=> u_u;
        } else {
            if (t_t < u_u) {
                return std::weak_ordering::less;
            }
            if (u_u < t_t) {
                return std::weak_ordering::greater;
            }
            return std::weak_ordering::equivalent;
        }
    };

    template <class T, class U = T>
    using synth_three_way_result =
        decltype(synth_three_way(std::declval<T&>(), std::declval<U&>()));
}; // namespace cpplib
