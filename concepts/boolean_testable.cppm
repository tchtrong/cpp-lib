export module cpplib:concepts.boolean_testable;

import :concepts.convertible_to;

namespace cpplib {
    template <typename B>
    concept boolean_testable_impl = convertible_to<B, bool>;

    template <typename B>
    concept boolean_testable = boolean_testable_impl<B> && requires(B&& b_b) {
        { !std::forward<B>(b_b) } -> boolean_testable_impl;
    };
} // namespace cpplib