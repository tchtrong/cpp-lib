export module cpplib:concepts.destructible;

import std;

namespace cpplib {
    export template <typename T>
    concept maythrow_destructible = std::is_destructible_v<T>;

    export template <typename T>
    concept trivially_destructible = std::is_trivially_destructible_v<T>;

    export template <typename T>
    concept destructible = std::is_nothrow_destructible_v<T>;
} // namespace cpplib