export module cpplib:concepts.same_as;

import std;

namespace cpplib {
    export template <typename T, typename U>
    concept same_as = std::is_same_v<T, U> && std::is_same_v<U, T>;
} // namespace cpplib