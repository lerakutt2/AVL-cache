#pragma once
#include "Leaf.h"
#include "Test/Functions.h"
#include "Tree.h"
#include <any>
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

template <typename T> class Tree;
template <typename T> class Leaf;

/// <summary>
/// Manager of Linked List of Trees (LLT)
/// </summary>
/// <typeparam name="ReturnType">Type of function return value</typeparam>
/// <typeparam name="...InputTypes">Types of function input
/// parameters</typeparam>
template <typename ReturnType, typename... InputTypes> class TreeList {
public:
  // trees typed by function parameters types
  using PossibleTypes = std::variant<Tree<InputTypes> *...>;

  /// <summary>
  /// Item of LLT
  /// </summary>
  struct Link {
    PossibleTypes tree;
    std::unique_ptr<Link> next;
    Link(PossibleTypes tr) : tree(std::move(tr)) {}
  };

  /// <summary>
  /// Linked List of Trees (LLT)
  /// </summary>
  class VariantList {
  public:
    VariantList() = default;
    ~VariantList() = default;

    VariantList(const VariantList &) = delete;
    VariantList &operator=(const VariantList &) = delete;
    VariantList(VariantList &&) noexcept = default;
    VariantList &operator=(VariantList &&) noexcept = default;

    std::unique_ptr<Link> head = nullptr;
    Link *current = nullptr;
    Link *tail = nullptr; // O(1) push_back

    /// <summary>
    /// Add element to the beginning of the list
    /// </summary>
    /// <param name="value">element to add</param>
    template <typename T> void push_front(T value) {
      static_assert(std::is_constructible_v<PossibleTypes, T>,
                    "Type not supported in this list");
      auto node = std::make_unique<Link>(std::forward(value));
      if (!head) {
        tail = node.get();
      }
      node->next = std::move(head);
      head = std::move(node);
    }

    /// <summary>
    /// Add element to the back of the list
    /// </summary>
    /// <param name="value">element to add</param>
    template <typename T> void push_back(T &&value) {
      static_assert(std::is_constructible_v<PossibleTypes, T>,
                    "Type not supported in this list");

      auto node = std::make_unique<Link>(std::forward<T>(value));
      auto *raw_node = node.get();
      if (!head) {
        head = std::move(node);
      } else {
        tail->next = std::move(node);
      }
      tail = raw_node;
    }

    /// Delete and get first element
    void pop_front() {
      if (!head)
        return;
      if (head.get() == tail) {
        tail = nullptr;
      }
      head = std::move(head->next);
    }

    /// <summary>
    /// Get element at index
    /// </summary>
    PossibleTypes &at(size_t index) {
      auto *curr = head.get();
      for (size_t i = 0; i < index && curr; ++i) {
        curr = curr->next.get();
      }
      if (!curr)
        throw std::out_of_range("Index out of range");
      return curr->tree;
    }

    /// Move from current item to next
    void move_next() {
      if (current)
        current = current->next.get();
    }

    /// <summary>
    /// Print all element of list
    /// </summary>
    void print_all() const {
      for (auto *curr = head.get(); curr; curr = curr->next.get()) {
        std::visit(
            [](auto *tree) {
              if (tree)
                std::cout << "Tree[" << typeid(*tree).name() << "]";
              else
                std::cout << "null";
            },
            curr->tree);

        if (curr->next)
          std::cout << " -> ";
      }
      std::cout << " -> null\n";
    }

    bool empty() const noexcept { return head == nullptr; }

    void clear() noexcept {
      head.reset();
      tail = nullptr;
      current = nullptr;
    }
  };

  explicit TreeList(ReturnType (*func)(InputTypes...));
  ~TreeList() = default;

  TreeList(const TreeList &) = delete;
  TreeList &operator=(const TreeList &) = delete;
  TreeList(TreeList &&) noexcept = default;
  TreeList &operator=(TreeList &&) noexcept = default;

  /// <summary>
  /// LLT for the particular function the class is created for
  /// </summary>
  VariantList treelist;

  using FirstType = std::tuple_element_t<0, std::tuple<InputTypes...>>;
  static constexpr int paramsCount = sizeof...(InputTypes);
  int unique;

  /// <summary>
  /// Pointer to a function
  /// </summary>
  std::function<ReturnType(InputTypes...)> func_;

  /// <summary>
  /// Root leaf of the first tree in LLT
  /// </summary>
  std::unique_ptr<Leaf<FirstType>> head = std::make_unique<Leaf<FirstType>>();

  // tuple of trees
  std::tuple<Tree<InputTypes>...> trees;

  // tuple of parameters (updates each time parameters are passed in GetValue)
  std::tuple<InputTypes...> paramsTuple;

  void CreateTrees();
  ReturnType GetValue(InputTypes... data);

  template <size_t idx> void CreateTreeList();

  template <size_t idx> void SetValues(const std::tuple<InputTypes...> &data);
};

// deduction guide
template <typename ReturnType, typename... InputTypes>
TreeList(ReturnType (*)(InputTypes...)) -> TreeList<ReturnType, InputTypes...>;

// Implementation
#include "TreeList_impl.h"