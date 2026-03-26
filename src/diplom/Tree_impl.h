#pragma once
#include <cstdlib>
#include <iostream>
#include <functional>
#include <tuple>

//// Constructor
//template<typename T>
//Tree<T>::Tree() {}
//// Destructor
//template<typename T>
//Tree<T>::~Tree() {
//    //if (head.left) CleanTree(head.left);
//    //if (head.right) CleanTree(head.right);
//    //if (head.next) CleanTree(head.next);
//}

template<typename T>
Leaf<T>* Tree<T>::CreateLeaf(T arg) {
    return new Leaf<T>(arg);
}

//
//// private GetValue
//template<T>
//void* Tree<T>::GetValue(int TreeLevel, Leaf<InputType>* head,
//    InputType* data) {
//    Leaf<InputType>* currHead = head;
//    Leaf<InputType>* currNode;
//
//    InputType functionParam;
//    for (; TreeLevel >= 0; --TreeLevel) { // cycle traversing all parameters (all Trees)
//        functionParam = data[TreeLevel];
//        currNode = currHead->next;
//
//        currNode = Find(currNode, functionParam);
//        if (currNode != nullptr) { // I have found requested node, either return result or go
//            if (TreeLevel == 0) {
//                std::cout << "Node found!" << std::endl;
//                return currNode->value;
//            }
//            else // one level further
//                currHead = currNode;
//        }
//        else { // I have not found requested node, add it and return result
//            Leaf<InputType>* result;
//            currHead->next = InsertNode(currHead->next, data, TreeLevel, result);
//            return result->value;
//        }
//    }
//    throw std::runtime_error("Tree::GetValue: unexpected state");
//}
//
//// Find <T> todo
//template<T>
//Leaf<T>* Tree<T>::Find(Leaf<T>* currNode, T searchVal)
//{
//    while (true) {               // cycle across all leaves on a given Tree
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
//// CleanTree <T> todo
//template<T>
//void Tree<T>::CleanTree(Leaf<T>* leaf) {
//    if (leaf == nullptr)
//        return;
//
//    Leaf<T>* leftChild = leaf->left;
//    Leaf<T>* rightChild = leaf->right;
//    Leaf<T>* nextChild = leaf->next;
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
//// NewNode <T> todo
//template<T>
//Leaf<InputType>* Tree<T>::NewNode(T* data, const int& TreeLevel,
//    Leaf<InputType>*& value) {
//    Leaf<InputType>* node = new Leaf<InputType>(data[TreeLevel]);
//    Leaf<InputType>* lastNode = node;
//
//    for (int idx = TreeLevel - 1; idx >= 0; --idx) {
//        lastNode->next = new Leaf<InputType>(data[idx]);
//        lastNode = lastNode->next;
//    }
//
//    std::cout << "New node, calling GG" << std::endl;
//    // now only for GG
//    ReturnType* tmpVal = new ReturnType;
//    int L_value = static_cast<int>(data[0]);
//    *tmpVal = GG(L_value, data[1], data[2], data[3], data[4]);
//    lastNode->value = static_cast<void*>(tmpVal);
//    value = lastNode;
//
//    return node;
//}
//
//// InsertNode <T> todo
//template<T>
//Leaf<InputType>* Tree<T>::InsertNode(Leaf<InputType>* node, InputType* data,
//    const int& TreeLevel, Leaf<InputType>*& value) {
//    if (node == nullptr)
//        return NewNode(data, TreeLevel, value);
//
//    if (data[TreeLevel] > node->argument)
//        node->right = InsertNode(node->right, data, TreeLevel, value);
//    else
//        node->left = InsertNode(node->left, data, TreeLevel, value);
//
//    return Balance(node, data[TreeLevel]);
//}
//
//// Balance
//template<T>
//Leaf<InputType>* Tree<T>::Balance(Leaf<InputType>* node, InputType argument) {
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
//template<T>
//int Tree<T>::Height(Leaf<InputType>* node) {
//    if (node == nullptr)
//        return 0;
//    return node->height;
//}
//
//// RightRotate
//template<T>
//Leaf<InputType>* Tree<T>::RightRotate(Leaf<InputType>* y) {
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
//template<T>
//Leaf<InputType>* Tree<T>::LeftRotate(Leaf<InputType>* x) {
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
//template<T>
//int Tree<T>::GetBalance(Leaf<InputType>* N) {
//    if (N == nullptr)
//        return 0;
//    return Height(N->left) - Height(N->right);
//}