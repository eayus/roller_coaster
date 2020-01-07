#pragma once

#include <iostream>

namespace std_extras {

    template <typename T, size_t N>
    struct Array {
        T elems[N];

        constexpr size_t size() const { return N; }
    };

    /*template <typename T, size_t N>
    class Array {
    public:
        typedef T* iterator;
        typedef const T* const_iterator;

        constexpr iterator begin() { return &this->x; }
        constexpr const_iterator begin() const { return &this->x; }
        constexpr iterator end() { return &this->last(); }
        constexpr const_iterator end() const { return &this->last(); }


        constexpr Array(T x, Array<T, N -1> xs)
            : x(x)
            , xs(xs) {}

        constexpr Array(T default_value)
            : x(default_value)
            , xs(default_value) {}

        constexpr T& operator[](int index) {
            return index == 0 ? this->x : xs[index - 1];
        }

        void print() const {
            std::cout << this->x << ", ";
            this->xs.print();
        }

        constexpr T& last() {
            return this->xs.last();
        }

        constexpr Array<T, N> map_add(T n) const {
            return Array<T, N>(
                this->x + n,
                this->xs.map_add(n)
            );
        }

        T x;
        Array<T, N - 1> xs;
    };

    template <typename T>
    class Array<T, 1> {
    public:
        constexpr Array(T default_value)
            : x(default_value) {}

        constexpr T& operator[](int index) {
            return this->x;
        }

        void print() const {
            std::cout << this->x << std::endl;
        }

        constexpr T& last() {
            return this->x;
        }

        constexpr Array<T, 1> map_add(T n) const {
            return Array<T, 1>(this->x + n);
        }

        T x;
    };


    template<typename T, size_t N, size_t M>
    constexpr Array<T, N + M> concat(const Array<T, N>& arr1, const Array<T, M>& arr2) {
        return Array<T, N + M>(arr1.x, concat(arr1.xs, arr2));
    }

    template<typename T, size_t N>
    constexpr Array<T, N + 1> concat(const Array<T, 1>& arr1, const Array<T, N>& arr2) {
        return Array<T, N + 1>(arr1.x, arr2);
    }*/
}
