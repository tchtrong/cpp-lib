
import std;
import cpplib;

using list       = cpplib::doubly_list<int>;
using const_list = const list;

auto iterator(int /*unused*/, char** /*unused*/) {
    static_assert(cpplib::legacy_bidirectional_iterator<list::iterator>);
    static_assert(cpplib::legacy_bidirectional_iterator<list::const_iterator>);
    static_assert(cpplib::legacy_bidirectional_iterator<list::reverse_iterator>);
    static_assert(cpplib::legacy_bidirectional_iterator<list::const_reverse_iterator>);
    static_assert(cpplib::convertible_to<list::iterator, list::const_iterator>);
    static_assert(not cpplib::convertible_to<list::const_iterator, list::iterator>);
    static_assert(std::equality_comparable_with<list::iterator, list::const_iterator>);
    return 0;
}
