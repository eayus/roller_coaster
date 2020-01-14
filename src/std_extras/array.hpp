#pragma once

#include <iostream>

namespace std_extras {

    // Simply wrap an array so we can easily return it from a function
    template <typename T, size_t N>
    struct Array {
        T elems[N];

        constexpr size_t size() const { return N; }
    };

}
