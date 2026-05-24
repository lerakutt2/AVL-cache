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
        ((Is < paramsCount ?
            CreateTreeList<Is>() : void()), ...);
    }(std::index_sequence_for<InputTypes...>{});

}

template<typename ReturnType, typename... InputTypes>
template<size_t idx>
void TreeList<ReturnType, InputTypes...>::CreateTreeList() {
    treelist.push_back(PossibleTypes{ &std::get<idx>(trees) });
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

    Link* it = treelist.head.get(); // итератор по списку Link

    void* currHead = static_cast<void*>(&head->value);
    void* currNode = static_cast<void*>(head->value);

    //it = it->next.get();
    
    while (true) {
        currNode = std::visit([&](auto& tree) -> void* {
            return tree->FindAbstract(currNode); 
            }, it->tree);

        if (currNode == nullptr) break; // узел не найден
        if (!it->next.get()) { // нашли узел и это последнее дерево
            break;
        }
        currHead = currNode;
        currNode = *static_cast<void**>(currNode);
        it = it->next.get(); // Переходим к следующему дереву в списке
    }

    void* val;
    if (currNode == nullptr) {
        std::cout << "CAHCHE MISS\n";
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

        ReturnType* tmpRes = new ReturnType;
        *tmpRes = func_(params...);

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