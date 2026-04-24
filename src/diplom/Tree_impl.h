#pragma once
#include <cstdlib>
#include <iostream>
#include <functional>
#include <tuple>

template<typename T>
void Tree<T>::SetValue(T val) {
    searchValue = val;
}

template<typename T>
void Tree<T>::Link(Tree* tree) {
    nextTree = tree;
}

// private GetValue
template<typename T>
Leaf<T>* Tree<T>::GetValue(Leaf<T>* currHead) {
    Leaf<T>* currNode = currHead->next;
    currNode = Find(currNode, searchValue);
    if (currNode != nullptr) { // I have found requested node, either return result or go
        if (nextTree == nullptr) { // if current tree is the last one
            std::cout << "Node found!" << std::endl;
            return currNode;
        }
        else // one level further
            return nextTree->GetValue(currNode);
    }
    else { // I have not found requested node, add it and return it
        Leaf<T>* result;
        currHead->next = InsertNode(currHead->next, result);
        return result;
    }
    throw std::runtime_error("Tree::GetValue: unexpected state");

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

// NewNode <T> todo
template<typename T>
Leaf<T>* Tree<T>::NewNode(Leaf<T>* node, Leaf<T>*& lastLeaf) {
    node = new Leaf<T>(searchValue);

    if (nextTree != nullptr) // если текущее дерево не последнее
        node->next = nextTree->NewNode(node->next, lastLeaf);

    lastLeaf = node;
    return node;
}

// InsertNode <T> todo
template<typename T>
Leaf<T>* Tree<T>::InsertNode(Leaf<T>* node, Leaf<T>*& lastLeaf) {
    if (node == nullptr)
        return NewNode(node, lastLeaf);

    if (searchValue > node->argument)
        node->right = InsertNode(node->right, lastLeaf);
    else
        node->left = InsertNode(node->left, lastLeaf);

    return Balance(node);
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