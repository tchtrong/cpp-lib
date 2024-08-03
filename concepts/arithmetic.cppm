export module cpplib:concepts.arithmetic;

import std;

namespace cpplib {
    export template <typename T>
    concept arithmetic = std::is_arithmetic_v<T>;

    export template <typename T>
    concept signed_arithmetic = std::is_signed_v<T>;

    export template <typename T>
    concept integral = std::is_integral_v<T>;

    export template <typename T>
    concept signed_integral = integral<T> && signed_arithmetic<T>;

    export template <typename T>
    concept unsigned_integral = integral<T> && !signed_integral<T>;

    export template <typename T>
    concept floating_point = std::is_floating_point_v<T>;
} // namespace cpplib