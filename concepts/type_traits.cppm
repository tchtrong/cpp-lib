export module cpplib:concepts.type_traits;

import std;

namespace cpplib {
    export template <typename T>
    concept is_object = std::is_object_v<T>;

    export template <typename T>
    concept is_fundamental = std::is_fundamental_v<T>;

    export template <typename T>
    concept is_compound = !is_fundamental<T>;

    export template <typename T>
    concept is_void = is_fundamental<T> && std::is_void_v<T>;

    export template <typename T>
    concept is_function = is_compound<T> && std::is_function_v<T>;

    export template <typename T>
    concept is_scalar = is_object<T> && std::is_scalar_v<T>;

    export template <typename T>
    concept is_array = is_object<T> && is_compound<T> && std::is_array_v<T>;

    export template <typename T>
    concept is_union = is_object<T> && is_compound<T> && std::is_union_v<T>;

    export template <typename T>
    concept is_class = is_object<T> && is_compound<T> && std::is_class_v<T>;

    export template <typename T>
    concept is_null_pointer = is_fundamental<T> && is_scalar<T> && std::is_null_pointer_v<T>;

    export template <typename T>
    concept is_enum = is_scalar<T> && is_compound<T> && std::is_enum_v<T>;

    export template <typename T>
    concept is_pointer = is_scalar<T> && is_compound<T> && std::is_pointer_v<T>;

    export template <typename T>
    concept is_member_pointer = is_scalar<T> && is_compound<T> && std::is_member_pointer_v<T>;

    export template <typename T>
    concept is_member_function_pointer =
        is_member_pointer<T> && std::is_member_function_pointer_v<T>;

    export template <typename T>
    concept is_member_object_pointer = is_member_pointer<T> && std::is_member_object_pointer_v<T>;

    export template <typename T>
    concept is_const = std::is_const_v<T>;

    export template <typename Base, typename Derived>
    concept is_base_of = std::is_base_of_v<Base, Derived>;

} // namespace cpplib