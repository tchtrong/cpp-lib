export module cpplib:iterator;

import :concepts;

export namespace cpplib {
    template <typename I>
    concept legacy_iterator = requires(I iter) {
        { *iter } -> referenceable;
        { ++iter } -> same_as<I&>;
        { *iter++ } -> referenceable;
    }; // && copyable<I>;
} // namespace cpplib
