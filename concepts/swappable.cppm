export module cpplib:concepts.swappable;

import std;
import :concepts.reference;

namespace cpplib {
    export template <typename T>
    concept swappable = requires(T& lhs, T& rhs) { std::ranges::swap(lhs, rhs); };

    export template <typename T, typename U>
    concept swappable_with = common_reference_with<T, U> && requires(T&& first, U&& second) {
        std::ranges::swap(std::forward<T>(first), std::forward<T>(first));
        std::ranges::swap(std::forward<U>(second), std::forward<U>(second));
        std::ranges::swap(std::forward<T>(first), std::forward<U>(second));
        std::ranges::swap(std::forward<U>(second), std::forward<T>(first));
    };
} // namespace cpplib