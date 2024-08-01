#include <cassert>

import std;
import cpplib;

struct S {
    constexpr S() noexcept = default;

    constexpr explicit S(int x_in) noexcept : x(new(std::nothrow) int(x_in)) {}

    constexpr S(const S& s_in) noexcept : x(new(std::nothrow) int(*(s_in.x))) {}

    constexpr auto operator=(const S& s_in) noexcept -> S& {
        if (this == &s_in) {
            return *this;
        }
        delete x;
        x = new (std::nothrow) int(*s_in.x);
        return *this;
    }

    constexpr ~S() noexcept {
        delete x;
    }

    constexpr S(S&& s_in) noexcept : x(s_in.x) {
        s_in.x = nullptr;
    }

    constexpr auto operator=(S&& s_in) noexcept -> S& {
        x      = s_in.x;
        s_in.x = nullptr;
        return *this;
    };

    constexpr auto operator==(const S& rhs) const noexcept -> bool {
        return *x == *rhs.x;
    }

    int* x{};
};

using list       = cpplib::doubly_list<S>;
using const_list = const list;

auto doubly_list(int /*argc*/, char** /*argv*/) -> int {
    list list01{};
    assert(list01.size() == 0);

    auto end_iter = list01.end();
    auto tmp_iter = std::as_const(list01).begin();
    assert(tmp_iter == end_iter);

    list01.insert(list01.end(), S{1});
    assert(list01.size() == 1);
    assert(list01.front() == list01.back());
    assert(list01.front() == *list01.begin());
    tmp_iter = ++list01.begin();
    assert(tmp_iter == end_iter);

    list01.insert(list01.cbegin(), S{2});
    assert(list01.size() == 2);
    tmp_iter = list01.begin();
    assert(list01.front() == *tmp_iter);
    assert(*tmp_iter->x == 2);
    ++tmp_iter;
    assert(*tmp_iter->x == 1);
    ++tmp_iter;
    assert(tmp_iter == end_iter);

    return 0;
}