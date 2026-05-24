//#ifndef TreeList_H
//#define TreeList_H_
//#include <vector>
//#include <variant>
//#include "Functions.h"
//#include "Leaf.h"
//#include "Tree.h"
//#include <any>
//#include <memory>
//#include <string>
//#include <stdexcept>
//
//
//
//// Будем использовать вариативные шаблоны. Запись  typename... InputTypes значит, что шаблон может принять 0 или более типов в качестве своих аргументов.
///// <summary>
///// Класс, содержащий дерево для конкретной функции
///// </summary>
///// <typeparam name="ReturnType">Тип выходного параметра</typeparam>
///// <typeparam name="...InputTypes">Типы входных параметров (по порядку)</typeparam>
//template<typename ReturnType, typename... InputTypes>
//class TreeList {
//public:
//    using PossibleTypes = std::variant<Tree<InputTypes>...>;
//
//    struct Link {
//        PossibleTypes tree;
//        std::unique_ptr<Link> next;
//        VariantNode(PossibleTypes tr) : tree(std::move(tr)) {}
//    };
//
//    TreeList(ReturnType(*func)(InputTypes...));
//
//    ~TreeList();
//    //template<typename T, typename N>
//    //using TreePair = Link<T, N>;
//
//    //// Получаем N-й тип
//    //template<size_t I>
//    //using Nth = std::tuple_element_t<I, std::tuple<InputTypes...>>;
//
//    //// Если есть следующий - берём его, иначе void
//    //template<size_t I>
//    //using Next = std::conditional_t<
//    //    (I + 1 < sizeof...(InputTypes)),
//    //    Nth<I + 1>,
//    //    void
//    //    >;
//
//    //// Генерируем список индексов
//    //template<size_t... I>
//    //static auto TreeTuple(std::index_sequence<I...>)
//    //    -> std::tuple<TreePair<Nth<I>, Next<I>>...>;
//
//    //using Links = decltype(TreeTuple(std::index_sequence_for<InputTypes...>{}));
//    //Links trees;
//
//    TreeList(TreeList&& other) noexcept;
//    TreeList& operator=(TreeList&& other) noexcept;
//
//    ReturnType GetValue(InputTypes... data);
//
//private:
//    using FirstType = std::tuple_element_t<0, std::tuple<InputTypes...>>;
//    std::function<ReturnType(InputTypes...)> func_;
//    Leaf<FirstType> head;
//
//    static constexpr int paramsCount = sizeof...(InputTypes);
//
//    std::tuple<Tree<InputTypes>...> trees;
//    std::tuple<InputTypes...> paramsTuple;
//
//    void CreateTrees();
//
//    template<size_t idx>
//    void LinkTrees();
//
//    template<size_t idx>
//    void SetValues(const std::tuple<InputTypes...>& data);
//
//    //void CleanTree(Leaf<InputType>* leaf);
//};
//
//// deduction guide
//template<typename ReturnType, typename... InputTypes>
//TreeList(ReturnType(*)(InputTypes...)) -> TreeList<ReturnType, InputTypes...>;
//
//// Подключаем реализацию шаблонных методов
//#include "../../src/diplom/TreeList_impl.h"
//
//#endif // TreeList_H