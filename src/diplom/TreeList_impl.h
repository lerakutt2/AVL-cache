#pragma once
#include <cstdlib>
#include <iostream>
#include <functional>
#include <tuple>
#include <any>

template<typename ReturnType, typename... InputTypes>
TreeList<ReturnType, InputTypes...>::TreeList(ReturnType(*func)(InputTypes...))
    : func_(func)
{
    CreateTrees();
}

template<typename ReturnType, typename... InputTypes>
void TreeList<ReturnType, InputTypes...>::CreateTrees() {    
    trees = std::make_tuple(Tree<InputTypes>()...);

    [&] <size_t... Is>(std::index_sequence<Is...>) {
        (CreateTreeList<Is>(), ...);
    }(std::index_sequence_for<InputTypes...>{});

}

template<typename ReturnType, typename... InputTypes>
template<size_t idx>
void TreeList<ReturnType, InputTypes...>::CreateTreeList() {
    treelist.push_back(PossibleTypes{ std::in_place_index<idx>, &std::get<idx>(trees) });
}

// public GetValue
template<typename ReturnType, typename... InputTypes>
ReturnType TreeList<ReturnType, InputTypes...>::GetValue(InputTypes... params) {

    paramsTuple = std::make_tuple(params...);

    // обновляем в деревьях значения в соответствии с переданными параметрами
    [&] <size_t... Is>(std::index_sequence<Is...>) {
        ((Is < paramsCount ?
            SetValues<Is>(paramsTuple) : void()), ...);
    }(std::index_sequence_for<InputTypes...>{});

    auto* it = treelist.head.get(); // итератор по списку Link

    void* currHead = static_cast<void*>(&head->value);
    void* currNode = static_cast<void*>(head->value);
    
    while (it) {
        currNode = std::visit([&](auto& tree) -> void* {
            return tree->FindAbstract(currNode); 
            }, it->tree);

        if (currNode == nullptr) break; // узел не найден

        if (!it->next.get()) break; // нашли узел и это последнее дерево
            
        currHead = currNode;
        currNode = *static_cast<void**>(currNode);
        it = it->next.get(); // Переходим к следующему дереву в списке
    }

    void* val = nullptr;
    if (currNode == nullptr) {
        std::cout << "CAHCHE MISS, CALLING FUNCTION\n";
        currNode = std::visit([&](auto& tree) -> void* {
            void* tmp = tree->InsertNodeAbstract(currHead);

            void* updatedNode = *static_cast<void**>(currHead);
            return tmp;
            }, it->tree); // нашел место для узла, создал, вернул след. от него
            
        while (it->next.get()) {
            it = it->next.get(); // Переходим к следующему дереву в списке

            currNode = std::visit([&](auto& tree) -> void* {
                return tree->NewNodeAbstract(currNode);
                }, it->tree);
        }

        auto* tmpRes = new ReturnType(std::apply(func_, paramsTuple));

        // Записываем указатель на результат физически в поле value последнего узла
        *static_cast<void**>(currNode) = static_cast<void*>(tmpRes);

        val = tmpRes;
    }
    else if (!it->next.get()) { // нашли узел и это последнее дерево
        std::cout << "FOUND\n";
        val = *static_cast<void**>(currNode);
    }

    if (val == nullptr) {
        throw std::runtime_error("Critial: val is nullptr");
    }

    ReturnType* typedPtr = static_cast<ReturnType*>(val);
    return *typedPtr;
}

template<typename ReturnType, typename... InputTypes>
template<size_t idx>
void TreeList<ReturnType, InputTypes...>::SetValues(const std::tuple<InputTypes...>& data) {
    std::get<idx>(trees).SetValue(std::get<idx>(data));
}