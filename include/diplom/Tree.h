#pragma once
#include "Functions.h"
#include "Leaf.h"

// Будем использовать вариативные шаблоны. Запись  typename... InputTypes значит, что шаблон может принять 0 или более типов в качестве своих аргументов.
/// <summary>
/// Класс, содержащий дерево для конкретной функции (пока GG)
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

    //void GetValue(InputType* data, ReturnType& result);
    //Leaf<T>* CreateLeaf(T arg);

    Leaf<T>* CreateLeaf();
    Leaf<T>* CreateLeaf(T arg);

    //Leaf<InputType> head;
    //void* GetValue(int treeLevel, Leaf<InputType>* head, InputType* data);

    //void CleanTree(Leaf<InputType>* leaf);

    //Leaf<InputType>* NewNode(InputType* data, const int& treeLevel, Leaf<InputType>*& value);

    //Leaf<InputType>* InsertNode(Leaf<InputType>* node, InputType* data,
    //    const int& treeLevel, Leaf<InputType>*& value);

    Leaf<T>* Find(Leaf<T>* currNode, T searchVal);

    //Leaf<InputType>* Balance(Leaf<InputType>* node, InputType argument);

    //int Height(Leaf<InputType>* node);

    //Leaf<InputType>* RightRotate(Leaf<InputType>* y);

    //Leaf<InputType>* LeftRotate(Leaf<InputType>* x);

    //int GetBalance(Leaf<InputType>* N);
};

// Подключаем реализацию шаблонных методов
#include "../../src/diplom/Tree_impl.h"
