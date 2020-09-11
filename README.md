# A `if constexpr`alternative for c++14

```c++
template <typename T>
constexpr auto switch_test() {
  return
    ic::if_<std::is_integral<T>::value>([] {
        return 42;
    }, ic::else_if_<std::floating_point<T>::value>([] {
        return 42.2;
    }), ic::else_([] {
        return nullptr;    
    }));
}

int main() {
  assert(switch_test<int>() == 42);
  assert(switch_test<double>() == 42.2);
  assert(switch_test<nullptr>() == nullptr);
}
```

## Limitations

- Cannot be used in unevaluated context (due to lambda usage)
