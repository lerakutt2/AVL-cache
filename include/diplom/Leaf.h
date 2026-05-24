#pragma once
// Структура узла дерева
template<typename T>
struct Leaf {
    Leaf* left = nullptr;
    Leaf* right = nullptr;
    //Leaf* next = nullptr;
    int height = 1;
    T argument{};
    void* value = nullptr;

    Leaf() = default;
    explicit Leaf(const T& arg) : argument(arg) {}
};