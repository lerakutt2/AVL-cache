#pragma once
#include "Leaf.h"

/// <summary>
/// Template for a tree of specific type
/// </summary>
/// <typeparam name="T">Type of argument</typeparam>
template <typename T> class Tree {
public:
  Tree() = default;
  Tree(const Tree &) = default;
  Tree(Tree &&) = default;
  Tree &operator=(const Tree &) = default;
  Tree &operator=(Tree &&) = default;
  ~Tree() = default;

  T searchValue;
  T GetArgument(void *absNode);
  void SetSearchValue(T val);

  // abstract methods
  void *FindAbstract(void *abstractNode);
  void *InsertNodeAbstract(void *currHead);
  void *NewNodeAbstract(void *absNode);
  void SetNextAbstract(void *absNode, void *next);

  // typed methods
  Leaf<T> *Find(Leaf<T> *currNode, const T &value);
  Leaf<T> *NewNode(Leaf<T> *&node, Leaf<T> *&lastLeaf);
  Leaf<T> *InsertNode(Leaf<T> *&node, Leaf<T> *&lastLeaf);

  // Balancing
  Leaf<T> *Balance(Leaf<T> *node);
  int Height(Leaf<T> *node);
  Leaf<T> *RightRotate(Leaf<T> *y);
  Leaf<T> *LeftRotate(Leaf<T> *x);
  int GetBalance(Leaf<T> *N);
};

// Implementation
#include "Tree_impl.h"
