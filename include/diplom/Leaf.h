#pragma once
template<typename T>
struct Leaf {
    Leaf* left = nullptr;
    Leaf* right = nullptr;
    int height = 1; 

    /// value of function parameter
    T argument{};

    /// pointer to next level head or to the function result value
    void* next = nullptr; 

    Leaf() = default;
    explicit Leaf(const T& arg) : argument(arg) {}
};