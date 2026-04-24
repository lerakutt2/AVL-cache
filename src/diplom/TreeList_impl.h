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
            LinkTrees<Is>() : void()), ...);
    }(std::index_sequence_for<InputTypes...>{});

}


template<typename ReturnType, typename... InputTypes>
template<size_t idx>
void TreeList<ReturnType, InputTypes...>::LinkTrees() {
    if constexpr (idx + 1 < sizeof...(InputTypes)) {
        std::get<idx>(trees).Link(&std::get<idx + 1>(trees));
    }
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

    auto res = std::get<0>(trees).GetValue(&head);

    if (res->value == nullptr) {
        ReturnType* tmpRes = new ReturnType;
        *tmpRes = func_(params...);
        res->value = static_cast<void*>(tmpRes);
    }

    return *static_cast<ReturnType*>(res->value);
}

template<typename ReturnType, typename... InputTypes>
template<size_t idx>
void TreeList<ReturnType, InputTypes...>::SetValues(const std::tuple<InputTypes...>& data) {
    std::get<idx>(trees).SetValue(std::get<idx>(data));
}

//// CleanTree
//template<typename ReturnType, typename... InputTypes>
//void TreeList<ReturnType, InputTypes...>::CleanTree(Leaf<InputType>* leaf) {
//    if (leaf == nullptr)
//        return;
//
//    Leaf<InputType>* leftChild = leaf->left;
//    Leaf<InputType>* rightChild = leaf->right;
//    Leaf<InputType>* nextChild = leaf->next;
//
//    CleanTree(leftChild);
//    CleanTree(rightChild);
//    CleanTree(nextChild);
//
//    if (leaf->value != nullptr) {
//        delete static_cast<ReturnType*>(leaf->value);
//    }
//
//    delete leaf;
//}