#pragma once
#include <cstdlib>
#include <iostream>
#include <functional>
#include <tuple>

template<typename T>
void Tree<T>::SetValue(T val) {
    searchValue = val;
    std::cout << "SetValue " << val << ", " << searchValue << std::endl;

}

template<typename T>
void Tree<T>::Link(Tree* tree) {
    nextTree = tree;
}

template<typename T>
T Tree<T>::GetValue(void* absNode) {
    Leaf<T>* currNode = static_cast<Leaf<T>*>(absNode);
    return currNode == nullptr ? 0 : 1;
}

template<typename T>
void Tree<T>::SetValueAbstract(void* absNode, void* value) {
    Leaf<T>* currNode = static_cast<Leaf<T>*>(absNode);
    currNode->value = value;
}

template<typename T>
void* Tree<T>::InsertNodeAbstract(void* absHead) {
    //void** parentValuePtr = static_cast<void**>(absHead);
    Leaf<T>** parentNodePtr = static_cast<Leaf<T>**>(absHead);
    Leaf<T>* currHead = *parentNodePtr;

    //Leaf<T>* currHead = static_cast<Leaf<T>*>(absHead);
    std::cout << "currhead before " << (currHead ? currHead->argument : -1) << std::endl;

    Leaf<T>* result = nullptr;
    currHead = InsertNode(currHead, result);
    *parentNodePtr = currHead;

    std::cout << "result->argument " << result->argument << std::endl;
    std::cout << "currhead->argument " << currHead->argument << std::endl;

    return static_cast<void*>(&result->value);
}

template<typename T>
void* Tree<T>::NewNodeAbstract(void* absNode) {
    Leaf<T>** parentValuePtr = static_cast<Leaf<T>**>(absNode);

    Leaf<T>* result = nullptr;
    Leaf<T>* newNode = nullptr;

    NewNode(newNode, result);
    *parentValuePtr = newNode;

    std::cout << "New node result->argument " << newNode->argument << std::endl;
    std::cout << "Connected to parent address: " << parentValuePtr << std::endl;

    return static_cast<void*>(&result->value);
}

// NewNode <T> 
template<typename T>
Leaf<T>* Tree<T>::NewNode(Leaf<T>*& node, Leaf<T>*& lastLeaf) {
    node = new Leaf<T>(searchValue);
    lastLeaf = node;
    std::cout << "searchValue " << searchValue << std::endl;
    std::cout << "node->argument " << node->argument << std::endl;
    std::cout << "lastLeaf->argument " << lastLeaf->argument << std::endl;

    return node;
}

// InsertNode <T>
template<typename T>
Leaf<T>* Tree<T>::InsertNode(Leaf<T>*& node, Leaf<T>*& lastLeaf) {
    if (node == nullptr) {
        std::cout << "calling new node from insert" << std::endl;

        return NewNode(node, lastLeaf);
    }

    if (searchValue > node->argument) {
        std::cout << "gone right " << std::endl;
        node->right = InsertNode(node->right, lastLeaf);
    }
    else {
        std::cout << "gone left " << std::endl;
        node->left = InsertNode(node->left, lastLeaf);
    }
    return Balance(node);
}

template<typename T>
void* Tree<T>::FindAbstract(void* abstractNode) {
    Leaf<T>* specificNode = static_cast<Leaf<T>*>(abstractNode);
    Leaf<T>* found = Find(specificNode, searchValue);

    if (found == nullptr) return nullptr;

    return static_cast<void*>(&found->value);
}

// Find <T> 
template<typename T>
Leaf<T>* Tree<T>::Find(Leaf<T>* currNode, const T& value)
{
    {
        while (true) {       
            // cycle across all leaves on a given Tree
            if (currNode == nullptr) // I have not found given node, need to add
                return currNode;
            if (value == currNode->argument) {
                break;
            }
            else if (value < currNode->argument)
                currNode = currNode->left;
            else
                currNode = currNode->right;
        }                // cycle across all leaves

        return currNode;
    }
}


// Balance
template<typename T>
Leaf<T>* Tree<T>::Balance(Leaf<T>* node) {

    node->height = 1 + Max(Height(node->left), Height(node->right));
    int balance = GetBalance(node);

    // Левое-левое
    if (balance > 1 && searchValue < node->left->argument)
        return RightRotate(node);

    // Правое-правое
    if (balance < -1 && searchValue > node->right->argument)
        return LeftRotate(node);

    // Левое-правое
    if (balance > 1 && searchValue > node->left->argument) {
        node->left = LeftRotate(node->left);
        return RightRotate(node);
    }

    // Правое-левое
    if (balance < -1 && searchValue < node->right->argument) {
        node->right = RightRotate(node->right);
        return LeftRotate(node);
    }

    return node;
}

// Height
template<typename T>
int Tree<T>::Height(Leaf<T>* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

// RightRotate
template<typename T>
Leaf<T>* Tree<T>::RightRotate(Leaf<T>* y) {
    Leaf<T>* x = y->left;
    Leaf<T>* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = Max(Height(y->left), Height(y->right)) + 1;
    x->height = Max(Height(x->left), Height(x->right)) + 1;

    return x;
}

// LeftRotate
template<typename T>
Leaf<T>* Tree<T>::LeftRotate(Leaf<T>* x) {
    Leaf<T>* y = x->right;
    Leaf<T>* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = Max(Height(x->left), Height(x->right)) + 1;
    y->height = Max(Height(y->left), Height(y->right)) + 1;

    return y;
}

// GetBalance
template<typename T>
int Tree<T>::GetBalance(Leaf<T>* N) {
    if (N == nullptr)
        return 0;
    return Height(N->left) - Height(N->right);
}