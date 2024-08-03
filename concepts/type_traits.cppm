export module cpplib:concepts.type_traits;

import std;

namespace cpplib {
    export template <typename T>
    concept object = std::is_object_v<T>;

    export template <typename T>
    concept is_const = std::is_const_v<T>;

    export template <typename Base, typename Derived>
    concept base_of = std::is_base_of_v<Base, Derived>;
} // namespace cpplib