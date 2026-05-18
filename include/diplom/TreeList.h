#ifndef TreeList_H
#define TreeList_H_
#include <vector>
#include <variant>
#include "Functions.h"
#include "Leaf.h"
#include "Tree.h"
#include <any>
#include <memory>
#include <string>
#include <stdexcept>

// Будем использовать вариативные шаблоны. Запись  typename... InputTypes значит, что шаблон может принять 0 или более типов в качестве своих аргументов.
/// <summary>
/// Класс, содержащий дерево для конкретной функции
/// </summary>
/// <typeparam name="ReturnType">Тип выходного параметра</typeparam>
/// <typeparam name="...InputTypes">Типы входных параметров (по порядку)</typeparam>
template<typename ReturnType, typename... InputTypes>
class TreeList {
public:
    using PossibleTypes = std::variant<Tree<InputTypes>...>;

    struct Link {
        PossibleTypes tree;
        std::unique_ptr<Link> next;
        Link(PossibleTypes tr) : tree(std::move(tr)) {}
    };

    TreeList(ReturnType(*func)(InputTypes...));

    ~TreeList();

    class VariantList {
    public:
        std::unique_ptr<Link> head;
        std::unique_ptr<Link> current;

        // Добавление в начало
        template<typename T>
        void push_front(T value) {
            static_assert(std::is_constructible_v<PossibleTypes, T>,
                "Type not supported in this list");
            auto node = std::make_unique<Link>(std::move(value));
            node->next = std::move(head);
            head = std::move(node);
        }

        // Добавление в конец
        template<typename T>
        void push_back(T value) {
            auto new_node = std::make_unique<Link>(std::move(value));
            if (!head) {
                head = std::move(new_node);
                return;
            }
            auto* curr = head.get();
            while (curr->next) {
                curr = curr->next.get();
            }
            curr->next = std::move(new_node);
        }

        // Удаление первого элемента
        void pop_front() {
            if (head) {
                head = std::move(head->next);
            }
        }

        // Доступ к элементу по индексу
        PossibleTypes& at(size_t index) {
            auto* curr = head.get();
            for (size_t i = 0; i < index && curr; ++i) {
                curr = curr->next.get();
            }
            if (!curr) throw std::out_of_range("Index out of range");
            return curr->value;
        }

        void move_next() {
            if (!current) {
                current = head.get();
            }
            current = current->next.get();
        }

        void print_all() const {
            for (auto* curr = head.get(); curr; curr = curr->next.get()) {
                curr->print();
                if (curr->next) std::cout << " -> ";
            }
            std::cout << " -> null\n";
        }

        bool empty() const { return head == nullptr; }

        void clear() { head.reset(); }
    };

    VariantList treelist;
    using FirstType = std::tuple_element_t<0, std::tuple<InputTypes...>>;

    std::function<ReturnType(InputTypes...)> func_;   
    Leaf<FirstType> head;
        static constexpr int paramsCount = sizeof...(InputTypes);

    std::tuple<Tree<InputTypes>...> trees;
    void CreateTrees();

    template<size_t idx>
    void CreateTreeList();
    
    template<size_t idx>
    void SetValues(const std::tuple<InputTypes...>& data);
        ReturnType GetValue(InputTypes... data);
     std::tuple<InputTypes...> paramsTuple;


};
    //template<typename T, typename N>
    //using TreePair = Link<T, N>;

    //// Получаем N-й тип
    //template<size_t I>
    //using Nth = std::tuple_element_t<I, std::tuple<InputTypes...>>;

    //// Если есть следующий - берём его, иначе void
    //template<size_t I>
    //using Next = std::conditional_t<
    //    (I + 1 < sizeof...(InputTypes)),
    //    Nth<I + 1>,
    //    void
    //    >;

    //// Генерируем список индексов
    //template<size_t... I>
    //static auto TreeTuple(std::index_sequence<I...>)
    //    -> std::tuple<TreePair<Nth<I>, Next<I>>...>;

    //using Links = decltype(TreeTuple(std::index_sequence_for<InputTypes...>{}));
    //Links trees;

//    TreeList(TreeList&& other) noexcept;
//    TreeList& operator=(TreeList&& other) noexcept;
//
//    ReturnType GetValue(InputTypes... data);
//
//private:
//    std::function<ReturnType(InputTypes...)> func_;   
//    Leaf<FirstType> head;
//

//
//
//    template<size_t idx>
//    void LinkTrees();
//
//    template<size_t idx>
//    void SetValues(const std::tuple<InputTypes...>& data);
//
//    //void CleanTree(Leaf<InputType>* leaf);
//};

// deduction guide
template<typename ReturnType, typename... InputTypes>
TreeList(ReturnType(*)(InputTypes...)) -> TreeList<ReturnType, InputTypes...>;

// Подключаем реализацию шаблонных методов
#include "../../src/diplom/TreeList_impl.h"

#endif // TreeList_H