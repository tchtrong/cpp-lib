#include "cpplib_doubly_list.hpp"

#include <cassert>
#include <new>

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

static constexpr cpplib::array<int, 10> input{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

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

    list01.erase(tmp_iter);
    assert(list01.size() == 2);
    --tmp_iter;
    list01.erase(tmp_iter);
    tmp_iter = --list01.end();
    assert(tmp_iter == list01.begin());

    list01.clear();
    assert(list01.size() == 0);
    tmp_iter = list01.begin();
    assert(tmp_iter == end_iter);

    list01.insert(list01.end(), input.begin(), input.end());
    assert(list01.size() == input.size());
    auto idx = 0;
    for (auto&& ele : std::as_const(list01)) {
        assert(*ele.x == idx);
        ++idx;
    }

    list01.erase(list01.cbegin(), list01.cend());
    assert(list01.size() == 0);
    tmp_iter = list01.begin();
    assert(tmp_iter == end_iter);

    return 0;
}