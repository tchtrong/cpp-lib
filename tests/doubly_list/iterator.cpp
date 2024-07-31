#include "cpplib_doubly_list.hpp"

using list       = cpplib::doubly_list<int>;
using const_list = const list;

auto iterator(int /*unused*/, char** /*unused*/) {
    static_assert(std::bidirectional_iterator<list::iterator>);
    static_assert(std::bidirectional_iterator<list::const_iterator>);
    static_assert(std::bidirectional_iterator<list::reverse_iterator>);
    static_assert(std::bidirectional_iterator<list::const_reverse_iterator>);
    static_assert(std::is_convertible_v<list::iterator, list::const_iterator>);
    static_assert(not std::is_convertible_v<list::const_iterator, list::iterator>);
    static_assert(std::equality_comparable_with<list::iterator, list::const_iterator>);

    return 0;
}
