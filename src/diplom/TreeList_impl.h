#pragma once
#include <cstdlib>
#include <iostream>
#include <functional>
#include <tuple>
#include <any>

template<typename ReturnType, typename... InputTypes>
TreeList<ReturnType, InputTypes...>::TreeList(ReturnType(*func)(InputTypes...)) {
    func_ = func;
    CreateTrees();
}

// Destructor
template<typename ReturnType, typename... InputTypes>
TreeList<ReturnType, InputTypes...>::~TreeList() {
    //CleanTree(head);
}

template<typename ReturnType, typename... InputTypes>
void TreeList<ReturnType, InputTypes...>::CreateTrees() {    
    trees = std::make_tuple(Tree<InputTypes>()...);
    [&] <size_t... Is>(std::index_sequence<Is...>) {
        ((Is < paramsCount - 1 ?
            CreateTreeList<Is>() : void()), ...);
    }(std::index_sequence_for<InputTypes...>{});

}

template<typename ReturnType, typename... InputTypes>
template<size_t idx>
void TreeList<ReturnType, InputTypes...>::CreateTreeList() {
    if constexpr (idx + 1 < sizeof...(InputTypes)) {
        treelist.push_back(PossibleTypes{ std::get<idx>(trees)});
    }
}

// public GetValue
template<typename ReturnType, typename... InputTypes>
ReturnType TreeList<ReturnType, InputTypes...>::GetValue(InputTypes... params) {

    paramsTuple = std::make_tuple(params...);

    // обновл€ем в деревь€х значени€ в соответствии с переданными параметрами
    [&] <size_t... Is>(std::index_sequence<Is...>) {
        ((Is < paramsCount ?
            SetValues<Is>(paramsTuple) : void()), ...);
    }(std::index_sequence_for<InputTypes...>{});

    //auto res = std::get<0>(trees).GetValue(&head);
    Link* it = treelist.head.get(); // итератор по списку Link
    void* currNode = static_cast<void*>(&head); // Ќачинаем поиск с корн€ (nullptr)
    //it = it->next.get();
    
    while (it) {
        // «ахватываем по ссылке [&], чтобы измен€ть currNode на каждой итерации
        currNode = std::visit([&](auto& tree) -> void* {
            // ѕередаем сам указатель (void*), а не его адрес
            return tree.FindAbstract(currNode);
            }, it->tree);

        it = it->next.get(); // ѕереходим к следующему дереву в списке
    }
    // auto currentLink = treelist.head.get();
    //int val = std::get<0>(paramsTuple);

    //// »спользуем std::visit дл€ вызова метода Find
    //std::visit([&](auto& tree) {
    //    using T = typename std::decay_t<decltype(tree)>; // ƒостаем тип T из Tree<T>

    //    if constexpr (std::is_same_v<T, FirstType>) {
    //        // Ёта ветка скомпилируетс€ только дл€ типа FirstType
    //        auto* result = tree.Find(&head, val);
    //    }
    //    else {
    //        // Ћогика дл€ остальных типов (например, ошибка или другой head)
    //        // tree.Find(...) здесь вызывать нельз€, так как типы не совпадут
    //    }
    //    }, currentLink->tree);

   /* auto currNode = treelist.head.get()->tree->Find(&head, treelist->head->searchValue);
    treelist.move_next();
    while(treelist.current) {
        auto currNode = treelist.current.get()->tree->Find(currNode, treelist->current->searchValue);
    }
    if (currNode->value == nullptr) {
        ReturnType* tmpRes = new ReturnType;
        *tmpRes = func_(params...);
        currNode->value = static_cast<void*>(tmpRes);
    }

    return *static_cast<ReturnType*>(currNode->value);*/
}

template<typename ReturnType, typename... InputTypes>
template<size_t idx>
void TreeList<ReturnType, InputTypes...>::SetValues(const std::tuple<InputTypes...>& data) {
    std::get<idx>(trees).SetValue(std::get<idx>(data));
}

////// CleanTree
////template<typename ReturnType, typename... InputTypes>
////void TreeList<ReturnType, InputTypes...>::CleanTree(Leaf<InputType>* leaf) {
////    if (leaf == nullptr)
////        return;
////
////    Leaf<InputType>* leftChild = leaf->left;
////    Leaf<InputType>* rightChild = leaf->right;
////    Leaf<InputType>* nextChild = leaf->next;
////
////    CleanTree(leftChild);
////    CleanTree(rightChild);
////    CleanTree(nextChild);
////
////    if (leaf->value != nullptr) {
////        delete static_cast<ReturnType*>(leaf->value);
////    }
////
////    delete leaf;
////}