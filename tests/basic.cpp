#include <if_constexpr.hpp>

#include <cassert>

template <typename T>
constexpr auto switch_test() {
    return
        ic::if_<std::is_integral<T>::value>([] {
            return 42;
        }, ic::else_if_<std::is_floating_point<T>::value>([] {
            return 42.2;
        }));
}

//static_assert(switch_test<int>() == 42, ""); // works in c++17
//static_assert(switch_test<double>() == 42.2, ""); // works in c++17
//static_assert(switch_test<void>() == nullptr, ""); // works in c++17

static_assert(std::is_same<decltype(switch_test<int>()), int>::value, "");
static_assert(std::is_same<decltype(switch_test<double>()), double>::value, "");
static_assert(std::is_same<decltype(switch_test<void>()), void>::value, "");


template <typename T>
constexpr auto switch_test2() {
    return
        ic::switch_(
            ic::case_<std::is_integral<T>::value>([] {
                return 42;
            }),
            ic::case_<std::is_floating_point<T>::value>([] {
                return 42.2;
            }),
            // default is always true, so it must be last
            ic::default_([] {
                return nullptr;
            })
        );
}

static_assert(std::is_same<decltype(switch_test2<int>()), int>::value, "");
static_assert(std::is_same<decltype(switch_test2<double>()), double>::value, "");
static_assert(std::is_same<decltype(switch_test2<void>()), std::nullptr_t>::value, "");

int main() {
    assert(switch_test<int>() == 42);
    assert(switch_test<double>() == 42.2);

    assert(switch_test2<int>() == 42);
    assert(switch_test2<double>() == 42.2);
    assert(switch_test2<void>() == nullptr);
    return 0;
}
