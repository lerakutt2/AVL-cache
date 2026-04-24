#ifndef TreeList_H
#define TreeList_H_
#include <vector>
#include <variant>
#include "Functions.h"
#include "Leaf.h"
#include "Tree.h"
#include <any>
// Будем использовать вариативные шаблоны. Запись  typename... InputTypes значит, что шаблон может принять 0 или более типов в качестве своих аргументов.
/// <summary>
/// Класс, содержащий дерево для конкретной функции
/// </summary>
/// <typeparam name="ReturnType">Тип выходного параметра</typeparam>
/// <typeparam name="...InputTypes">Типы входных параметров (по порядку)</typeparam>
template<typename ReturnType, typename... InputTypes>
class TreeList {
public:
    TreeList(ReturnType(*func)(InputTypes...));

    ~TreeList();

    TreeList(TreeList&& other) noexcept;
    TreeList& operator=(TreeList&& other) noexcept;

    ReturnType GetValue(InputTypes... data);

private:
    std::function<ReturnType(InputTypes...)> func_;   
    Leaf<int> head;
    using tupleOfTypes = std::tuple<InputTypes...>;
    int paramsCount = sizeof...(InputTypes);
    std::tuple<Tree<InputTypes>...> trees;
    InputTypes params...;
    std::tuple<InputTypes...> paramsTuple = std::tuple<params...>;

    void CreateTrees();
    void* GetValue(int TreeLevel);

    //void CleanTree(Leaf<InputType>* leaf);

    // Вспомогательная функция для создания одного узла на уровне idx
    template<size_t idx>
    void* createNodeAtLevel(const std::tuple<InputTypes...>& data);

    // Рекурсивное связывание узлов
    template<size_t idx>
    void linkNodes(void* nodes[], const std::tuple<InputTypes...>& data);

    // Создание всех узлов
    template<size_t... Is>
    void createAllNodes(void* nodes[], const std::tuple<InputTypes...>& data,
        std::index_sequence<Is...>);

    void* NewNode(const int& TreeLevel, Leaf<int>*& value);

    //template<size_t... Is>
    //auto CreateLeaves(std::index_sequence<Is...>) -> decltype(auto);

    //template<size_t... Is>
    //void ConnectLeavesRecursive(auto& leaves, int treeLevel, std::index_sequence<Is...>);

    //Leaf<InputType>* InsertNode(Leaf<InputType>* node, InputType* data,
     //   const int& treeLevel, Leaf<InputType>*& value);

    /*Leaf<InputType>* Find(Leaf<InputType>* currNode, InputType searchVal);

    Leaf<InputType>* Balance(Leaf<InputType>* node, InputType argument);

    int Height(Leaf<InputType>* node);

    Leaf<InputType>* RightRotate(Leaf<InputType>* y);

    Leaf<InputType>* LeftRotate(Leaf<InputType>* x);

    int GetBalance(Leaf<InputType>* N);*/
};

// deduction guide
template<typename ReturnType, typename... InputTypes>
TreeList(ReturnType(*)(InputTypes...)) -> TreeList<ReturnType, InputTypes...>;

// Подключаем реализацию шаблонных методов
#include "../../src/diplom/TreeList_impl.h"

#endif // TreeList_H