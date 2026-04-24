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
    using FirstType = std::tuple_element_t<0, std::tuple<InputTypes...>>;
    std::function<ReturnType(InputTypes...)> func_;   
    Leaf<FirstType> head;

    static constexpr int paramsCount = sizeof...(InputTypes);

    std::tuple<Tree<InputTypes>...> trees;
    std::tuple<InputTypes...> paramsTuple;

    void CreateTrees();
    void* GetValue(int TreeLevel);

    template<size_t idx>
    void LinkTrees();

    template<size_t idx>
    void SetValues(const std::tuple<InputTypes...>& data);

    //void CleanTree(Leaf<InputType>* leaf);
};

// deduction guide
template<typename ReturnType, typename... InputTypes>
TreeList(ReturnType(*)(InputTypes...)) -> TreeList<ReturnType, InputTypes...>;

// Подключаем реализацию шаблонных методов
#include "../../src/diplom/TreeList_impl.h"

#endif // TreeList_H