#pragma once
// Структура узла дерева
template<typename T>
struct Leaf {
    Leaf* left = nullptr;
    Leaf* right = nullptr;
    int height = 1; 
    T argument{}; // value of function parameter
    void* value = nullptr; // pointer to next level head or to function result value

    Leaf() = default;
    explicit Leaf(const T& arg) : argument(arg) {}
};