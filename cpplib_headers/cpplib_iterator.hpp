#ifndef CPPLIB_ITERATOR_H_
#define CPPLIB_ITERATOR_H_

#include "cpplib_concepts.hpp"

namespace cpplib {
    template <typename I>
    concept legacy_iterator = requires(I iter) {
        { *iter } -> referenceable;
        { ++iter } -> same_as<I&>;
        { *iter++ } -> referenceable;
    }; // && copyable<I>;
} // namespace cpplib

#endif