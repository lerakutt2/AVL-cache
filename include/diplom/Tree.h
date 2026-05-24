#pragma once
#include "Functions.h"
#include "Leaf.h"

// Будем использовать вариативные шаблоны. Запись  typename... InputTypes значит, что шаблон может принять 0 или более типов в качестве своих аргументов.
/// <summary>
/// Класс, содержащий дерево конкретного параметра функции
/// </summary>
/// <typeparam name="...Args">Типы входных параметров (по порядку)</typeparam>
/// <typeparam name="ReturnType">Тип выходного параметра</typeparam>
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
    Tree* nextTree = nullptr;

    void SetValue(T val);

    void Link(Tree* tree);

    Leaf<T>* NewNode(Leaf<T>*& node, Leaf<T>*& lastLeaf);
    Leaf<T>* InsertNode(Leaf<T>*& node, Leaf<T>*& lastLeaf);

    Leaf<T>* Balance(Leaf<T>* node);
    int Height(Leaf<T>* node);
    Leaf<T>* RightRotate(Leaf<T>* y);
    Leaf<T>* LeftRotate(Leaf<T>* x);
    int GetBalance(Leaf<T>* N);

    Leaf<T>* Find(Leaf<T>* currNode, const T& value);

    void* FindAbstract(void* abstractNode);
    
    void* InsertNodeAbstract(void* currHead);
    void* NewNodeAbstract(void* absNode);
    void SetValueAbstract(void* absNode, void* value);
    T GetValue(void* absNode);
    void* Next(void* absNode);

};

// Подключаем реализацию шаблонных методов
#include "../../src/diplom/Tree_impl.h"
