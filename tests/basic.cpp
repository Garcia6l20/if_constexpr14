#include <if_constexpr.hpp>

#include <type_traits>

#include <cassert>

template <typename T>
constexpr auto switch_test() {
    return
        ic::if_<std::is_integral<T>::value>([] {
            return 42;
        }, [] {
            return 42.2;
        });/*ic::else_if_<std::floating_point<T>::value>([] {
            return 42.2;
        }), ic::else_([] {
            return nullptr;
        }));*/
}

int main() {
    assert(switch_test<int>() == 42);
    assert(switch_test<double>() == 42.2);
    assert(switch_test<std::nullptr_t>() == 42.2);
    return 0;
}
