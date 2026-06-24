#pragma once
#include <cstdlib>
#include <functional>
#include <iostream>
#include <tuple>

/// <summary>
/// Set searchValue
/// </summary>
template <typename T> void Tree<T>::SetSearchValue(T val) { searchValue = val; }

/// <summary>
/// Get the argument in the Leaf
/// </summary>
/// <param name="absNode">Node of type void* that certainly of type T*</param>
/// <returns>absNode->argument</returns>
template <typename T> T Tree<T>::GetArgument(void *absNode) {
  Leaf<T> *currNode = static_cast<Leaf<T> *>(absNode);
  return currNode == nullptr ? T{} : currNode->argument;
}

/// <summary>
/// Set the next field of the node
/// </summary>
/// <param name="absNode">Node of type void* that certainly of type T*</param>
/// <param name="next">Next tree or value to set to node</param>
template <typename T> void Tree<T>::SetNextAbstract(void *absNode, void *next) {
  Leaf<T> *currNode = static_cast<Leaf<T> *>(absNode);
  currNode->next = next;
}

/// <summary>
/// Calls InsertNode with typed parameters
/// </summary>
/// <param name="absHead">Node of type void* that certainly of type T*</param>
/// <returns>Head of a tree next to created leaf or a function value</returns>
template <typename T> void *Tree<T>::InsertNodeAbstract(void *absHead) {
  auto **headPtr = static_cast<void **>(absHead);
  Leaf<T> *currHead = static_cast<Leaf<T> *>(*headPtr);

  Leaf<T> *newLeaf = nullptr;
  currHead = InsertNode(currHead, newLeaf);
  *headPtr = currHead;

  return static_cast<void *>(&newLeaf->next);
}

/// <summary>
/// Calls NewNode with typed parameters
/// </summary>
/// <param name="absNode"></param>
/// <returns>Head of a tree next to created leaf or a function value</returns>
template <typename T> void *Tree<T>::NewNodeAbstract(void *absNode) {
  Leaf<T> **parentValuePtr = static_cast<Leaf<T> **>(absNode);

  Leaf<T> *result = nullptr;
  Leaf<T> *newNode = nullptr;

  NewNode(newNode, result);
  *parentValuePtr = newNode;
  return static_cast<void *>(&result->next);
}

/// <summary>
/// Creates and returns new node with argument=searchValue
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="node">Empty node to fill</param>
/// <param name="lastLeaf">Empty node to fill</param>
/// <returns>New node</returns>
template <typename T>
Leaf<T> *Tree<T>::NewNode(Leaf<T> *&node, Leaf<T> *&lastLeaf) {
  node = new Leaf<T>(searchValue);
  lastLeaf = node;
  return node;
}

/// <summary>
/// Finds a place for a new node, starting with head
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="node">Empty node to fill</param>
/// <param name="lastLeaf">Empty node to fill</param>
/// <returns>New Node</returns>
template <typename T>
Leaf<T> *Tree<T>::InsertNode(Leaf<T> *&node, Leaf<T> *&lastLeaf) {
  if (node == nullptr) {
    return NewNode(node, lastLeaf);
  }

  if (searchValue > node->argument) {
    node->right = InsertNode(node->right, lastLeaf);
  } else {
    node->left = InsertNode(node->left, lastLeaf);
  }
  return Balance(node);
}

/// <summary>
/// Calls Find() with typed node
/// </summary>
/// <param name="abstractNode">Node of type void* that certainly has type
/// T</param> <returns>Pointer to next level head</returns>
template <typename T> void *Tree<T>::FindAbstract(void *abstractNode) {
  Leaf<T> *specificNode = static_cast<Leaf<T> *>(abstractNode);
  Leaf<T> *found = Find(specificNode, searchValue);

  if (found == nullptr)
    return nullptr;

  return static_cast<void *>(&found->next);
}

/// <summary>
/// Searches node with given value on a tree and returns it
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="currNode"></param>
/// <param name="value"></param>
/// <returns></returns>
template <typename T>
Leaf<T> *Tree<T>::Find(Leaf<T> *currNode, const T &value) {
  {
    while (true) {
      // cycle across all leaves on a given Tree
      if (currNode == nullptr) // I have not found given node, need to add
        return currNode;
      if (value == currNode->argument) {
        break;
      } else if (value < currNode->argument)
        currNode = currNode->left;
      else
        currNode = currNode->right;
    }
    return currNode;
  }
}

template <typename T> Leaf<T> *Tree<T>::Balance(Leaf<T> *node) {

  node->height = 1 + std::max(Height(node->left), Height(node->right));
  int balance = GetBalance(node);

  // Left-left
  if (balance > 1 && searchValue < node->left->argument)
    return RightRotate(node);

  // Right-right
  if (balance < -1 && searchValue > node->right->argument)
    return LeftRotate(node);

  // Left-right
  if (balance > 1 && searchValue > node->left->argument) {
    node->left = LeftRotate(node->left);
    return RightRotate(node);
  }

  // Right-left
  if (balance < -1 && searchValue < node->right->argument) {
    node->right = RightRotate(node->right);
    return LeftRotate(node);
  }

  return node;
}

// Height
template <typename T> int Tree<T>::Height(Leaf<T> *node) {
  return node == nullptr ? 0 : node->height;
}

// RightRotate
template <typename T> Leaf<T> *Tree<T>::RightRotate(Leaf<T> *y) {
  Leaf<T> *x = y->left;
  Leaf<T> *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = std::max(Height(y->left), Height(y->right)) + 1;
  x->height = std::max(Height(x->left), Height(x->right)) + 1;

  return x;
}

// LeftRotate
template <typename T> Leaf<T> *Tree<T>::LeftRotate(Leaf<T> *x) {
  Leaf<T> *y = x->right;
  Leaf<T> *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = std::max(Height(x->left), Height(x->right)) + 1;
  y->height = std::max(Height(y->left), Height(y->right)) + 1;

  return y;
}

// GetBalance
template <typename T> int Tree<T>::GetBalance(Leaf<T> *N) {
  if (N == nullptr)
    return 0;
  return Height(N->left) - Height(N->right);
}