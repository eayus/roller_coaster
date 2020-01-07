#include <iostream>
#include "array.hpp"

constexpr int plus_one(int x) { return x + 1; }

int main() {
    constexpr auto a = std_extras::TEST_ARRAY;
    constexpr auto b = std_extras::Array<int, 3>(10);
    constexpr auto c = b.map(&plus_one);
    constexpr auto d = std_extras::concat(a, c);
    constexpr std_extras::Array<bool, 3> e = { true, { false, { true } } };
    d.print();
    e.print();


    return 0;
}
