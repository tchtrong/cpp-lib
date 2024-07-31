#include "cpplib_doubly_list.hpp"
#include <cassert>

static constexpr cpplib::array<int, 6> input1{
    1, 3, 5, 7, 9, 12,
};
static constexpr cpplib::array<int, 8> input2{0, 2, 4, 6, 8, 10, 11, 13};

auto merge(int /*unused*/, char** /*unused*/) {
    cpplib::doubly_list<int> list01{cpplib::array{input1}};
    cpplib::doubly_list<int> list02{cpplib::array{input2}};
    list01.merge(list02);
    assert(list01.size() == input1.size() + input2.size());
    assert(list02.size() == 0);
    assert(list02.begin() == list02.end());
    auto idx = 0;
    for (auto&& ele : std::as_const(list01)) {
        assert(ele == idx);
        ++idx;
    }
    assert(list01.back() == input2.back());

    return 0;
}