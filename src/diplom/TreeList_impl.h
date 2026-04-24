#pragma once
#include <cstdlib>
#include <iostream>
#include <functional>
#include <tuple>
#include <any>

template<typename ReturnType, typename... InputTypes>
TreeList<ReturnType, InputTypes...>::TreeList(ReturnType(*func)(InputTypes...)) {
    func_ = func;
    //head = *std::get<0>(trees).CreateLeaf();
    CreateTrees();
}

// Destructor
template<typename ReturnType, typename... InputTypes>
TreeList<ReturnType, InputTypes...>::~TreeList() {
    //if (head.left) CleanTree(head.left);
    //if (head.right) CleanTree(head.right);
    //if (head.next) CleanTree(head.next);
}

template<typename ReturnType, typename... InputTypes>
void TreeList<ReturnType, InputTypes...>::CreateTrees() {
    trees = std::make_tuple(Tree<InputTypes>()...);
}

// public GetValue
template<typename ReturnType, typename... InputTypes>
ReturnType TreeList<ReturnType, InputTypes...>::GetValue(InputTypes... Inputdata) {
    params = Inputdata...;
    void* res = GetValue(paramsCount - 1);
    //result = *static_cast<ReturnType*>(res);
    
    return func_(data...);
}

// private GetValue
template<typename ReturnType, typename... InputTypes>
void* TreeList<ReturnType, InputTypes...>::GetValue(int TreeLevel) {
    auto currHead = &head;
    std::cout << TreeLevel << std::endl;
    //Leaf<std::get<0>(tupleOfTypes)>* currNode;
    for (; TreeLevel >= 0; --TreeLevel) { // cycle traversing all parameters (all TreeLists)
        auto currNode = currHead->next;
        //    auto currNode = std::get<0>(trees).CreateLeaf();
        currNode = std::get<1>(trees).Find(currNode, std::get<1>(data));
        if (currNode != nullptr) { // I have found requested node, either return result or go
            if (TreeLevel == 0) {
                std::cout << "Node found!" << std::endl;
                // return currNode->value;
            }
           else // one level further
                currHead = currNode;
        }
        else { // I have not found requested node, add it and return result
            Leaf<int>* result;
            //currHead->next = InsertNode(currHead->next, data, TreeLevel, result);
            currHead->next = std::any_cast<Leaf<int>*>(NewNode(TreeLevel, result, dataTuple));
            std::cout << "Null";
            //return result->value;
        }
    }    //throw std::runtime_error("TreeList::GetValue: unexpected state");
}

// Вспомогательная функция для создания одного узла на уровне idx
template<typename ReturnType, typename... InputTypes>
template<size_t idx>
void* TreeList<ReturnType, InputTypes...>::createNodeAtLevel(const std::tuple<InputTypes...>& data) {
    return std::get<idx>(trees).CreateLeaf(std::get<idx>(data));
}

// Рекурсивное связывание узлов
template<typename ReturnType, typename... InputTypes>
template<size_t idx>
void TreeList<ReturnType, InputTypes...>::linkNodes(void* nodes[], const std::tuple<InputTypes...>& data) {
    if constexpr (idx > 0) {
        using CurrentType = std::tuple_element_t<idx, tupleOfTypes>;
        using NextType = std::tuple_element_t<idx - 1, tupleOfTypes>;
        auto* current = static_cast<Leaf<CurrentType>*>(nodes[idx]);
        auto* next = static_cast<Leaf<NextType>*>(nodes[idx - 1]);
        current->next = next;
        linkNodes<idx - 1>(nodes, data);
    }
}

// Создание всех узлов
template<typename ReturnType, typename... InputTypes>
template<size_t... Is>
void TreeList<ReturnType, InputTypes...>::createAllNodes(void* nodes[], const std::tuple<InputTypes...>& data,
    std::index_sequence<Is...>) {
    ((nodes[Is] = createNodeAtLevel<Is>(data)), ...);
}

template<typename ReturnType, typename... InputTypes>
void* TreeList<ReturnType, InputTypes...>::NewNode(const int& TreeLevel, Leaf<int>*& value) {
    dataTuple = std::make_tuple(params);
    void* nodes[paramsCount];

    createAllNodes(nodes, dataTuple, std::index_sequence_for<InputTypes...>{});

    // Связываем только до TreeLevel
    [&] <size_t... Is>(std::index_sequence<Is...>) {
        ((Is == static_cast<size_t>(TreeLevel) ?
            linkNodes<Is>(nodes, dataTuple) : void()), ...);
    }(std::index_sequence_for<InputTypes...>{});

    // Получаем верхний
    void* node = nodes[TreeLevel];

    ReturnType* tmpVal = new ReturnType;
    *tmpVal = func_(params...);
    node->value = static_cast<void*>(tmpVal);

    return node;
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

// NewNode <T> todo
//template<typename ReturnType, typename... InputTypes>
//std::any TreeList<ReturnType, InputTypes...>::NewNode(const int& TreeLevel,
//    Leaf<int>*& value, std::tuple<InputTypes...> data) {
//    void* node = std::get<TreeLevel>(trees).CreateLeaf(std::get<TreeLevel>(data));
//    void* lastNode = node;
//
//    for (int idx = TreeLevel - 1; idx >= 0; --idx) {
//        lastNode->next = std::get<idx>(tree).CreateLeaf(std::get<idx>(data));
//        lastNode = lastNode->next;
//    }
//
//    //std::cout << "New node, calling GG" << std::endl;
//    //// now only for GG
//    //ReturnType* tmpVal = new ReturnType;
//    //int L_value = static_cast<int>(data[0]);
//    //*tmpVal = GG(L_value, data[1], data[2], data[3], data[4]);
//    //lastNode->value = static_cast<void*>(tmpVal);
//    //value = lastNode;
//
//    //return node;
//}

//// NewNode <T> todo
//template<typename ReturnType, typename... InputTypes>
//std::any TreeList<ReturnType, InputTypes...>::NewNode(const int& TreeLevel,
//    Leaf<int>*& value, std::tuple<InputTypes...> data) {
//    //Leaf<InputType>* node = new Leaf<InputType>(data[TreeLevel]);
//    //std::any lastNode = std::get<TreeLevel>(trees).CreateLeaf(std::get<TreeLevel>(data...));
//    //std::vector<std::any> newLeaves; //сделать список типов
//    //newLeaves.push_back(node);
//
//    //for (int idx = TreeLevel - 1; idx >= 0; --idx) {
//    //    newLeaves.push_back(std::get<idx>(trees).CreateLeaf()); //tree[i].createLeaf
//    //}
//    auto leaves = CreateLeaves(std::make_index_sequence<sizeof...(InputTypes)>{});
//
//    //for (int i = 0; i < TreeLevel - 1; ++i) { 
//    //    using CurrentType = std::tuple_element_t<i, tupleOfTypes>;
//    //    using NextType = std::tuple_element_t<i - 1, tupleOfTypes>;
//
//    //    //auto* currentLeaf = static_cast<CurrentType*>(std::get<i>(leaves));
//    //    //auto* nextLeaf = static_cast<NextType*>(std::get<i + 1>(leaves));
//    //    auto* currentLeaf = static_cast<CurrentType*>(leaves[i]);
//    //    auto* nextLeaf = static_cast<NextType*>(leaves[i+1]);
//    //    currentLeaf->next = nextLeaf;
//    //}
//    ConnectLeavesRecursive(leaves, TreeLevel, std::make_index_sequence<sizeof...(InputTypes)>{});
//
//    //for (int idx = TreeLevel - 1; idx >= 0; --idx) {
//    //    lastNode->next = std::get<idx>(tree).CreateLeaf(std::get<idx>(data)); //last->next = tree[idx].create(data[idx])
//    //    lastNode = lastNode->next;
//    //}
//
//    std::cout << "New node, calling GG" << std::endl;
//    // now only for GG
//    //ReturnType* tmpVal = new ReturnType;
//    //int L_value = static_cast<int>(data[0]);
//    //*tmpVal = GG(L_value, data[1], data[2], data[3], data[4]);
//    //auto* lastNode = std::any_cast<Leaf<InputType>*>(newLeaves[newLeaves.size() - 1]); //tree.last->value=tmpval
//    //lastNode->value = static_cast<void*>(10);
//    //value = lastNode;
//
//    //return lastNode;
//}

//template<typename ReturnType, typename... InputTypes>
//template<size_t... Is>
//void TreeList<ReturnType, InputTypes...>::ConnectLeavesRecursive(auto& leaves, int treeLevel, std::index_sequence<Is...>) {
//    // Создаем массив указателей для runtime доступа
//    std::array<void*, sizeof...(InputTypes)> leafPtrs = { std::get<Is>(leaves)... };
//
//    for (int idx = treeLevel - 1; idx > 0; --idx) {
//        using CurrentType = std::tuple_element_t<idx, tupleOfTypes>;
//        using NextType = std::tuple_element_t<idx - 1, tupleOfTypes>;
//
//        auto* currentLeaf = static_cast<CurrentType*>(leafPtrs[idx]);
//        auto* nextLeaf = static_cast<NextType*>(leafPtrs[idx - 1]);
//        currentLeaf->next = nextLeaf;
//    }
//}
//
//template<typename ReturnType, typename... InputTypes>
//template<size_t... Is>
//auto TreeList<ReturnType, InputTypes...>::CreateLeaves(std::index_sequence<Is...>) -> decltype(auto) {
//    return std::make_tuple(std::get<Is>(trees).CreateLeaf()...);
//}

//// InsertNode <T> todo
//template<typename ReturnType, typename... InputTypes>
//Leaf<InputType>* TreeList<ReturnType, InputTypes...>::InsertNode(Leaf<InputType>* node, InputType* data,
//    const int& TreeLevel, Leaf<InputType>*& value) {
//    if (node == nullptr)
//        return NewNode(data, TreeLevel, value);
//
//    if (data[TreeLevel] > node->argument)
//        node->right = InsertNode(node->right, data, TreeLevel, value);
//    else
//        node->left = InsertNode(node->left, data, TreeLevel, value);
//
//   return Balance(node, data[TreeLevel]);
//}
//
//// Balance
//template<typename ReturnType, typename... InputTypes>
//Leaf<InputType>* TreeList<ReturnType, InputTypes...>::Balance(Leaf<InputType>* node, InputType argument) {
//
//    node->height = 1 + Max(Height(node->left), Height(node->right));
//    int balance = GetBalance(node);
//
//    // Левое-левое
//    if (balance > 1 && argument < node->left->argument)
//        return RightRotate(node);
//
//    // Правое-правое
//    if (balance < -1 && argument > node->right->argument)
//        return LeftRotate(node);
//
//    // Левое-правое
//    if (balance > 1 && argument > node->left->argument) {
//        node->left = LeftRotate(node->left);
//        return RightRotate(node);
//    }
//
//    // Правое-левое
//    if (balance < -1 && argument < node->right->argument) {
//        node->right = RightRotate(node->right);
//        return LeftRotate(node);
//    }
//
//    return node;
//}
//
//// Height
//template<typename ReturnType, typename... InputTypes>
//int TreeList<ReturnType, InputTypes...>::Height(Leaf<InputType>* node) {
//    if (node == nullptr)
//        return 0;
//    return node->height;
//}
//
//// RightRotate
//template<typename ReturnType, typename... InputTypes>
//Leaf<InputType>* TreeList<ReturnType, InputTypes...>::RightRotate(Leaf<InputType>* y) {
//    Leaf<InputType>* x = y->left;
//    Leaf<InputType>* T2 = x->right;
//
//    x->right = y;
//    y->left = T2;
//
//    y->height = Max(Height(y->left), Height(y->right)) + 1;
//    x->height = Max(Height(x->left), Height(x->right)) + 1;
//
//    return x;
//}
//
//// LeftRotate
//template<typename ReturnType, typename... InputTypes>
//Leaf<InputType>* TreeList<ReturnType, InputTypes...>::LeftRotate(Leaf<InputType>* x) {
//    Leaf<InputType>* y = x->right;
//    Leaf<InputType>* T2 = y->left;
//
//    y->left = x;
//    x->right = T2;
//
//    x->height = Max(Height(x->left), Height(x->right)) + 1;
//    y->height = Max(Height(y->left), Height(y->right)) + 1;
//
//    return y;
//}
//
//// GetBalance
//template<typename ReturnType, typename... InputTypes>
//int TreeList<ReturnType, InputTypes...>::GetBalance(Leaf<InputType>* N) {
//    if (N == nullptr)
//        return 0;
//    return Height(N->left) - Height(N->right);
//}