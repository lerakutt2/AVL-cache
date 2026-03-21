#pragma once
#include <list>

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
/// <typeparam name="...InputTypes">Типы входных параметров (по порядку)</typeparam>
/// <typeparam name="ReturnType">Тип выходного параметра</typeparam>
template<typename ReturnType, typename... InputTypes>
class TreeList {
public:
    explicit TreeList(std::function<ReturnType(InputTypes...)> func);
    TreeList();
    ~TreeList();

    void GetValue(InputType* data, ReturnType& retData);

private:
    std::list<Tree> ListOfTrees;
    std::function<ReturnType(InputTypes...)> func;

    Leaf<int> head; // вместо int - std::tuple<InputTypes...>[0] - тип первого аргумента
    void* GetValue(int actLevel, Leaf<InputType>* head,
        InputType* data, Leaf<InputType>** lastLevelHead);

    void CleanTree(Leaf<InputType>* leaf);
    // класс дерева и класс содержащий все деревья? да!!!!!!!
    Leaf<InputType>* NewNode(InputType* data, const int& actLevel,
        Leaf<InputType>*& value,
        Leaf<InputType>** lastLevelHead);

    Leaf<InputType>* InsertNode(Leaf<InputType>* node, InputType* data,
        const int& actLevel, Leaf<InputType>*& value,
        Leaf<InputType>** lastLevelHead);

    int Height(Leaf<InputType>* node);

    Leaf<InputType>* RightRotate(Leaf<InputType>* y);

    Leaf<InputType>* LeftRotate(Leaf<InputType>* x);

    int GetBalance(Leaf<InputType>* N);
};

// Подключаем реализацию шаблонных методов
#include "../../src/diplom/TreeList_impl.h"