
#include "cpplib_array.hpp"
#include "cpplib_doubly_list.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>

using list       = cpplib::doubly_list<int>;
using const_list = const list;

static constexpr cpplib::array input{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

auto erase_clear(int /*unused*/, char** /*unused*/) -> int {
    list list01{cpplib::array{input}};
    list01.erase(list01.begin(), list01.end());
    assert(list01.empty());
    assert(list01.begin() == list01.end());

    list01.insert(list01.end(), input.begin(), input.end());
    list01.erase(list01.begin(), std::next(list01.begin(), 3));
    assert(std::equal(list01.begin(), list01.end(), std::next(input.begin(), 3)));

    list01.clear();
    assert(list01.empty());
    assert(list01.begin() == list01.end());

    return 0;
}