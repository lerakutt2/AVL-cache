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
ReturnType TreeList<ReturnType, InputTypes...>::GetValue(InputTypes... data) {
    void* res = GetValue(paramsCount, std::tuple<InputTypes...>(data...));
    //result = *static_cast<ReturnType*>(res);
    
    return func_(data...);
}

// private GetValue
template<typename ReturnType, typename... InputTypes>
void* TreeList<ReturnType, InputTypes...>::GetValue(int TreeLevel, std::tuple<InputTypes...> data) {
    //Leaf<InputType>* currHead = head;
    //Leaf<std::get<0>(tupleOfTypes)>* currNode;
   // for (; TreeLevel >= 0; --TreeLevel) { // cycle traversing all parameters (all TreeLists)
       // currNode = currHead->next;
    auto currNode = std::get<0>(trees).CreateLeaf();
        currNode = std::get<0>(trees).Find(currNode, std::get<0>(data));
        if (currNode != nullptr) { // I have found requested node, either return result or go
            if (TreeLevel == 0) {
                std::cout << "Node found!" << std::endl;
               // return currNode->value;
            }
            //else // one level further
               // currHead = currNode;
        }
        else { // I have not found requested node, add it and return result
            //Leaf<InputType>* result;
            //currHead->next = InsertNode(currHead->next, data, TreeLevel, result);
            std::cout << "Null";
            //return result->value;
        }
    //}
    //throw std::runtime_error("TreeList::GetValue: unexpected state");
}

//// Find <T> todo
//template<typename ReturnType, typename... InputTypes>
//Leaf<InputType>* TreeList<ReturnType, InputTypes...>::Find(Leaf<InputType>* currNode, InputType searchVal)
//{
//    while (true) {               // cycle across all leaves on a given TreeList
//        if (currNode == nullptr) // I have not found given node, need to add
//            return nullptr;
//        if (searchVal == currNode->argument) {
//            break;
//        }
//        else if (searchVal < currNode->argument)
//            currNode = currNode->left;
//        else
//            currNode = currNode->right;
//    }                // cycle across all leaves
//
//    return currNode;
//    
//}
//
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
//
//
//// NewNode <T> todo
//template<typename ReturnType, typename... InputTypes>
//Leaf<InputType>* TreeList<ReturnType, InputTypes...>::NewNode(InputType* data, const int& TreeLevel,
//    Leaf<InputType>*& value) {
//    Leaf<InputType>* node = new Leaf<InputType>(data[TreeLevel]);
//    // tree[1].create убрать
//    std::vector<std::any> newLeaves;
//    newLeaves.push_back(node);
//
//    for (int idx = TreeLevel - 1; idx >= 0; --idx) {
//        newLeaves.push_back(new Leaf<InputType>(data[idx])); //tree[i].createLeaf
//    }
//
//    for (std::size_t i = 0; i < newLeaves.size() - 1; ++i) { 
//        auto* leaf = std::any_cast<Leaf<InputType>*>(newLeaves[i]);
//        auto* nextLeaf = std::any_cast<Leaf<InputType>*>(newLeaves[i + 1]);
//        std::cout << i << " = " << static_cast<double>(leaf->argument) << " " << std::endl;
//        leaf->next = nextLeaf;
//    }
//
//    //for (int idx = TreeLevel - 1; idx >= 0; --idx) {
//    //    lastNode->next = new Leaf<InputType>(data[idx]);
//    //    lastNode = lastNode->next;
//    //}
//
//    std::cout << "New node, calling GG" << std::endl;
//    // now only for GG
//    ReturnType* tmpVal = new ReturnType;
//    int L_value = static_cast<int>(data[0]);
//    *tmpVal = GG(L_value, data[1], data[2], data[3], data[4]);
//    auto* lastNode = std::any_cast<Leaf<InputType>*>(newLeaves[newLeaves.size() - 1]); //tree.last->value=tmpval
//    lastNode->value = static_cast<void*>(tmpVal);
//    value = lastNode;
//
//    return node;
//}
//
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