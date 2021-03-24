link_libraries(if::constexpr)

add_executable(if-constexpr-test-basic basic.cpp)
add_test(NAME if-constexpr-test-basic COMMAND if-constexpr-test-basic)
