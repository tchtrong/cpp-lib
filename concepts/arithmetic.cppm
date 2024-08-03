export module cpplib:concepts.arithmetic;

import std;
import :concepts.type_traits;

namespace cpplib {

    export template <typename T>
    concept arithmetic = is_fundamental<T> && is_scalar<T> && std::is_arithmetic_v<T>;

    export template <typename T>
    concept signed_arithmetic = arithmetic<T> && std::is_signed_v<T>;

    export template <typename T>
    concept unsigned_arithmetic = arithmetic<T> && !signed_arithmetic<T>;

    export template <typename T>
    concept integral = arithmetic<T> && std::is_integral_v<T>;

    export template <typename T>
    concept signed_integral = integral<T> && signed_arithmetic<T>;

    export template <typename T>
    concept unsigned_integral = integral<T> && unsigned_arithmetic<T>;

    export template <typename T>
    concept floating_point = arithmetic<T> && std::is_floating_point_v<T>;

} // namespace cpplib