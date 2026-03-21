#ifndef TREE_H
#define TREE_H_
#include "Functions.h"


// Структура узла дерева
template<typename T>
struct Leaf {
    Leaf* left = nullptr;
    Leaf* right = nullptr;
    Leaf* next = nullptr;
    int height = 1;
    T argument{};
    void* value = nullptr;

    Leaf() = default;
    explicit Leaf(const T& arg) : argument(arg) {}
};

// Будем использовать вариативные шаблоны. Запись  typename... InputTypes значит, что шаблон может принять 0 или более типов в качестве своих аргументов.
/// <summary>
/// Класс, содержащий дерево для конкретной функции (пока GG)
/// </summary>
/// <typeparam name="...Args">Типы входных параметров (по порядку)</typeparam>
/// <typeparam name="ReturnType">Тип выходного параметра</typeparam>
template<typename InputType, typename ReturnType>
class Tree {
public:
    Tree();

    ~Tree();

    Tree(Tree&& other) noexcept;
    Tree& operator=(Tree&& other) noexcept;

    void GetValue(InputType* data, ReturnType& result);

private:
    Leaf<InputType> head;
    void* GetValue(int treeLevel, Leaf<InputType>* head, InputType* data);

    void CleanTree(Leaf<InputType>* leaf);

    Leaf<InputType>* NewNode(InputType* data, const int& treeLevel, Leaf<InputType>*& value);

    Leaf<InputType>* InsertNode(Leaf<InputType>* node, InputType* data,
        const int& treeLevel, Leaf<InputType>*& value);

    Leaf<InputType>* Find(Leaf<InputType>* currNode, InputType searchVal, bool& nodeFound);

    Leaf<InputType>* Balance(Leaf<InputType>* node, InputType argument);

    int Height(Leaf<InputType>* node);

    Leaf<InputType>* RightRotate(Leaf<InputType>* y);

    Leaf<InputType>* LeftRotate(Leaf<InputType>* x);

    int GetBalance(Leaf<InputType>* N);
};

// Подключаем реализацию шаблонных методов
#include "../../src/diplom/Tree_impl.h"

#endif // TREE_H