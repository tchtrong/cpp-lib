import std;
import cpplib;

#include <cassert>

auto main() -> int {
    cpplib::array       afsa{1, 2, 3};
    cpplib::doubly_list asdfc{cpplib::array{afsa}};
    afsa.front() = 2;
    assert(afsa.front() == 2);
    static_assert(std::is_same_v<decltype(afsa.data()), cpplib::array<int, 10>::pointer>);
    static_assert(std::is_same_v<decltype(std::as_const(afsa).data()),
                                 cpplib::array<int, 10>::const_pointer>);

    for (auto&& ele : std::as_const(afsa)) {
        std::cout << ele << '\n';
    }
    for (auto&& ele : std::as_const(asdfc)) {
        std::cout << ele << '\n';
    }
}
