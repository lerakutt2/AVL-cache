#pragma once
#include <cstdlib>
#include <iostream>
#include <functional>
#include <tuple>

// Constructor
template<typename InputType, typename ReturnType>
Tree<InputType, ReturnType>::Tree() {
    head.left = nullptr;
    head.right = nullptr;
    head.next = nullptr;
    head.value = nullptr;
    head.height = 1;
}

// Destructor
template<typename InputType, typename ReturnType>
Tree<InputType, ReturnType>::~Tree() {
    if (head.left) CleanTree(head.left);
    if (head.right) CleanTree(head.right);
    if (head.next) CleanTree(head.next);
}

// public GetValue
template<typename InputType, typename ReturnType>
void Tree<InputType, ReturnType>::GetValue(InputType* data, ReturnType& result) {
    void* res = GetValue(5, &head, data);
    result = *static_cast<ReturnType*>(res);
}

// private GetValue
template<typename InputType, typename ReturnType>
void* Tree<InputType, ReturnType>::GetValue(int treeLevel, Leaf<InputType>* head,
    InputType* data) {
    Leaf<InputType>* currHead = head;
    Leaf<InputType>* currNode;

    InputType functionParam;
    bool nodeFound; // I have found node with given properties (parameter value)
    for (; treeLevel >= 0; --treeLevel) { // cycle traversing all parameters (all trees)
        functionParam = data[treeLevel];
        currNode = currHead->next;

        currNode = Find(currNode, functionParam, nodeFound);
        if (nodeFound) { // I have found requested node, either return result or go
            if (treeLevel == 0) {
                std::cout << "Node found!" << std::endl;
                return currNode->value;
            }
            else // one level further
                currHead = currNode;
        }
        else { // I have not found requested node, add it and return result

            Leaf<InputType>* result;
            currHead->next = InsertNode(currHead->next, data, treeLevel, result);
            return result->value;
        }
    }
    throw std::runtime_error("Tree::GetValue: unexpected state");
}

template<typename InputType, typename ReturnType>
Leaf<InputType>* Tree<InputType, ReturnType>::Find(Leaf<InputType>* currNode, InputType searchVal, bool& nodeFound)
{
    nodeFound = false;
    while (1) {               // cycle across all leaves on a given tree
        if (currNode == nullptr) // I have not found given node, need to add
            break;
        if (searchVal == currNode->argument) {
            nodeFound = true;

            break;
        }
        else if (searchVal < currNode->argument)
            currNode = currNode->left;
        else
            currNode = currNode->right;
    }                // cycle across all leaves

    return currNode;
    
}
// CleanTree
template<typename InputType, typename ReturnType>
void Tree<InputType, ReturnType>::CleanTree(Leaf<InputType>* leaf) {
    if (leaf == nullptr)
        return;

    Leaf<InputType>* leftChild = leaf->left;
    Leaf<InputType>* rightChild = leaf->right;
    Leaf<InputType>* nextChild = leaf->next;

    CleanTree(leftChild);
    CleanTree(rightChild);
    CleanTree(nextChild);

    if (leaf->value != nullptr) {
        delete static_cast<ReturnType*>(leaf->value);
    }

    delete leaf;
}

// NewNode
template<typename InputType, typename ReturnType>
Leaf<InputType>* Tree<InputType, ReturnType>::NewNode(InputType* data, const int& treeLevel,
    Leaf<InputType>*& value) {
    Leaf<InputType>* node = new Leaf<InputType>(data[treeLevel]);
    Leaf<InputType>* lastNode = node;

    for (int idx = treeLevel - 1; idx >= 0; --idx) {
        lastNode->next = new Leaf<InputType>(data[idx]);
        lastNode = lastNode->next;
    }

    std::cout << "New node, calling GG" << std::endl;
    // now only for GG
    ReturnType* tmpVal = new ReturnType;
    int L_value = static_cast<int>(data[0]);
    *tmpVal = GG(L_value, data[1], data[2], data[3], data[4]);
    lastNode->value = static_cast<void*>(tmpVal);
    value = lastNode;

    return node;
}

// InsertNode
template<typename InputType, typename ReturnType>
Leaf<InputType>* Tree<InputType, ReturnType>::InsertNode(Leaf<InputType>* node, InputType* data,
    const int& treeLevel, Leaf<InputType>*& value) {
    if (node == nullptr)
        return NewNode(data, treeLevel, value);

    if (data[treeLevel] > node->argument)
        node->right = InsertNode(node->right, data, treeLevel, value);
    else
        node->left = InsertNode(node->left, data, treeLevel, value);

   return Balance(node, data[treeLevel]);
}

// Balance
template<typename InputType, typename ReturnType>
Leaf<InputType>* Tree<InputType, ReturnType>::Balance(Leaf<InputType>* node, InputType argument) {

    node->height = 1 + Max(Height(node->left), Height(node->right));
    int balance = GetBalance(node);

    // Левое-левое
    if (balance > 1 && argument < node->left->argument)
        return RightRotate(node);

    // Правое-правое
    if (balance < -1 && argument > node->right->argument)
        return LeftRotate(node);

    // Левое-правое
    if (balance > 1 && argument > node->left->argument) {
        node->left = LeftRotate(node->left);
        return RightRotate(node);
    }

    // Правое-левое
    if (balance < -1 && argument < node->right->argument) {
        node->right = RightRotate(node->right);
        return LeftRotate(node);
    }

    return node;
}

// Height
template<typename InputType, typename ReturnType>
int Tree<InputType, ReturnType>::Height(Leaf<InputType>* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

// RightRotate
template<typename InputType, typename ReturnType>
Leaf<InputType>* Tree<InputType, ReturnType>::RightRotate(Leaf<InputType>* y) {
    Leaf<InputType>* x = y->left;
    Leaf<InputType>* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = Max(Height(y->left), Height(y->right)) + 1;
    x->height = Max(Height(x->left), Height(x->right)) + 1;

    return x;
}

// LeftRotate
template<typename InputType, typename ReturnType>
Leaf<InputType>* Tree<InputType, ReturnType>::LeftRotate(Leaf<InputType>* x) {
    Leaf<InputType>* y = x->right;
    Leaf<InputType>* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = Max(Height(x->left), Height(x->right)) + 1;
    y->height = Max(Height(y->left), Height(y->right)) + 1;

    return y;
}

// GetBalance
template<typename InputType, typename ReturnType>
int Tree<InputType, ReturnType>::GetBalance(Leaf<InputType>* N) {
    if (N == nullptr)
        return 0;
    return Height(N->left) - Height(N->right);
}