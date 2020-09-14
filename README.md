# An `if constexpr`alternative for c++14

```c++
template <typename T>
constexpr auto switch_test() {
    return
        ic::if_<std::is_integral<T>::value>([] {
            return 42;
        }, ic::else_if_<std::is_floating_point<T>::value>([] {
            return 42.2;
        }, ic::else_([] {
            return nullptr;
        })));
}

//static_assert(switch_test<int>() == 42, ""); // works in c++17
//static_assert(switch_test<double>() == 42.2, ""); // works in c++17
//static_assert(switch_test<void>() == nullptr, ""); // works in c++17

static_assert(std::is_same<decltype(switch_test<int>()), int>::value, "");
static_assert(std::is_same<decltype(switch_test<double>()), double>::value, "");
static_assert(std::is_same<decltype(switch_test<void>()), std::nullptr_t>::value, "");

int main() {
    assert(switch_test<int>() == 42);
    assert(switch_test<double>() == 42.2);
    assert(switch_test<void>() == nullptr);
    return 0;
}
```

## Limitations

- Cannot be used in unevaluated context (due to lambda usage)
