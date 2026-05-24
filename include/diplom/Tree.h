#pragma once
#include "Functions.h"
#include "Leaf.h"

/// <summary>
/// Класс, содержащий дерево конкретного параметра функции
/// </summary>
/// <typeparam name="T">Тип параметра</typeparam>
template<typename T>
class Tree {
public:
    Tree() = default;
    Tree(const Tree&) = default;            // Копирующий
    Tree(Tree&&) = default;                 // Перемещающий
    Tree& operator=(const Tree&) = default; // Копирующее присваивание
    Tree& operator=(Tree&&) = default;      // Перемещающее присваивание
    ~Tree() = default;

    T searchValue;

    T GetValue(void* absNode);

    // abstract methods
    void* FindAbstract(void* abstractNode);
    void* InsertNodeAbstract(void* currHead);
    void* NewNodeAbstract(void* absNode);
    void SetValueAbstract(void* absNode, void* value);

    // typed methods
    Leaf<T>* Find(Leaf<T>* currNode, const T& value);
    Leaf<T>* NewNode(Leaf<T>*& node, Leaf<T>*& lastLeaf);
    Leaf<T>* InsertNode(Leaf<T>*& node, Leaf<T>*& lastLeaf);
    void SetValue(T val);


    // Balancing
    Leaf<T>* Balance(Leaf<T>* node);
    int Height(Leaf<T>* node);
    Leaf<T>* RightRotate(Leaf<T>* y);
    Leaf<T>* LeftRotate(Leaf<T>* x);
    int GetBalance(Leaf<T>* N);
};

// Подключаем реализацию шаблонных методов
#include "../../src/diplom/Tree_impl.h"
