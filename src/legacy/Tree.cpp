#include "Tree.h"

#include <cmath>
#include <cstring>
#include <iostream>

/// <summary>
/// Дерево, хранящее возсоэные различные параметры функции и возвращаемое значения
/// </summary>
/// <typeparam name="InputType">Тип входных данных</typeparam>
/// <typeparam name="ReturnType">Тип выходных данных</typeparam>
template<typename InputType, typename... Args>
class Tree {
    using InputType = std::tuple<Args...>;
public:
    ~Tree() {
        // Очистка всех корневых узлов
        if (headSinCos.next != nullptr) {
            CleanTree(headSinCos.next);
        }
    }

    Tree(Function f) : func(f) {}

    template<size_t... Is>
    ReturnType ComputeValue(InputType* data, std::index_sequence<Is...>) {
        return func(std::get<Is>(*data)...);
    }

    void GetValue(InputType* data) {
        GetValue(0, &headSinCos, data, nullptr);
    }

private:
    /// <summary>
    /// Проходит по каждому поддереву и ищет значение функции. Если не находит,то создает новый лист
    /// </summary>
    /// <param name="actLevel">Номер поддерева</param>
    /// <param name="head">Корень текущего дерева</param>
    /// <param name="data">Массив параметров (индекс = порядковый номер параметра в функции = номер поддерева)</param>
    /// <param name="lastLevelHead">Указатель на корень последнего использованного поддерева</param>
    /// <returns></returns>
    void* GetValue(int actLevel, Leaf* head, InputType* data, Leaf** lastLevelHead) {
        Leaf* actHead = head;
        Leaf* actNode;
        if (actLevel == 0) // starting from level 0 means, head is on "level 1"
            (*lastLevelHead) = head;
        else
            (*lastLevelHead) = nullptr;
        InputType actPar;
        bool nodeFound; // I have found node with given properties (parameter value)
        for (; actLevel > -1;
            --actLevel) { // cycle traversing all parameters (all trees)
            actPar = data[actLevel];
            actNode = actHead->next;
            nodeFound = false;
            while (1) {               // cycle across all leaves on a given tree
                if (actNode == nullptr) // I have not found given node, need to add
                    break;
                if (actPar == actNode->argument) {
                    nodeFound = true;
                    break;
                }
                else if (actPar < actNode->argument)
                    actNode = actNode->left;
                else
                    actNode = actNode->right;
            }                // cycle across all leaves
            if (nodeFound) { // I have found requested node, either return result or go
                // one level further
                if (actLevel == 0)
                    return actNode->value;
                else if (actLevel == 1) { // special case, storing pointer to 0-th level
                    // head for reuse
                    (*lastLevelHead) = actNode;
                    actHead = actNode;
                }
                else
                    actHead = actNode;
            }
            else { // I have not found requested node, add it and return result
                Leaf* result;
                actHead->next = InsertNode(actHead->next, data, actLevel, result,
                    lastLevelHead);
                return result->value;
            }
        }                   // cycle across all parameters
        exit(EXIT_FAILURE); // I should never come to this point as it means I haven't
        // found/added requested parameters set in(to) the tree
    }
    // sOneTreeLeaf headGG0;
    Leaf headSinCos;

    /// <summary>
    /// Рекурсивная очистка дерева, начиная с листа
    /// </summary>
    /// <param name="leaf">Ссылка на лист дерева</param>
    void CleanTree(Leaf*& leaf) {
        if (leaf == nullptr)
            return;

        // Рекурсивно очищаем потомков
        CleanTree(leaf->left);
        CleanTree(leaf->right);
        CleanTree(leaf->next);

        // Безопасно удаляем значение
        if (leaf->value != nullptr) {
            delete static_cast<ReturnType*>(leaf->value);
        }

        delete leaf;
        leaf = nullptr;
    }

    /* Helper function that allocates a new node with the given key and
   nullptr left and right pointers. */
   // пока только для GG
    Leaf* NewNode(InputType* data, const int& actLevel, Leaf*& value, Leaf** lastLevelHead) {
        Leaf* node = new Leaf;
        Leaf* lastNode;
        node->argument = data[actLevel];
        lastNode = node;
        if (actLevel == 1)
            (*lastLevelHead) = node;

        for (int idx = actLevel - 1; idx >= 0; --idx) {
            lastNode->next = new Leaf;
            lastNode = lastNode->next;
            lastNode->argument = data[idx];
            if (idx == 1)
                (*lastLevelHead) = lastNode;
        }

        ReturnType* tmpVal = new ReturnType;
        // в будущем будет любая функция с любыми параметрами
        *tmpVal = GG(static_cast<int>(data[0]), data[1], data[2], data[3], data[4]);
        lastNode->value = (void*)tmpVal;

        value = lastNode;
        return node;
    }

    Leaf* InsertNode(Leaf* node, InputType* data,
        const int& actLevel, Leaf*& value, Leaf** lastLevelHead) {
        /* 1. Perform the normal BST insertion */
        if (node == nullptr)
            return (NewNode(data, actLevel, value, lastLevelHead)); // !!!!

        // I'm not testing for equal node since I know that node is not there
        if (data[actLevel] > node->argument)
            node->right =
            InsertNode(node->right, data, actLevel, value, lastLevelHead);
        else
            node->left =
            InsertNode(node->left, data, actLevel, value, lastLevelHead);

        /* 2. Update height of this ancestor node */
        node->height = 1 + Max(Height(node->left), Height(node->right));

        /* 3. Get the balance factor of this ancestor
            node to check whether this node became
            unbalanced */
        int balance = GetBalance(node);

        // If this node becomes unbalanced, then
        // there are 4 cases

        // Left Left Case
        if (balance > 1 && data[actLevel] < node->left->argument)
            return RightRotate(node);

        // Right Right Case
        if (balance < -1 && data[actLevel] > node->right->argument)
            return LeftRotate(node);

        // Left Right Case
        if (balance > 1 && data[actLevel] > node->left->argument) {
            node->left = LeftRotate(node->left);
            return RightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && data[actLevel] < node->right->argument) {
            node->right = RightRotate(node->right);
            return LeftRotate(node);
        }
        /* return the (unchanged) node pointer */
        return node;
    }

    // A utility function to get the height of the tree
    int Height(Leaf* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    // A utility function to get maximum of two integers
    int Max(const int& a, const int& b) { return (a > b) ? a : b; }

    // A utility function to right
    // rotate subtree rooted with y
    // See the diagram given above.
    Leaf* RightRotate(Leaf* y) {
        Leaf* x = y->left;
        Leaf* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = Max(Height(y->left), Height(y->right)) + 1;
        x->height = Max(Height(x->left), Height(x->right)) + 1;

        // Return new root
        return x;
    }

    // A utility function to left
    // rotate subtree rooted with x
    // See the diagram given above.
    Leaf* LeftRotate(Leaf* x) {
        Leaf* y = x->right;
        Leaf* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = Max(Height(x->left), Height(x->right)) + 1;
        y->height = Max(Height(y->left), Height(y->right)) + 1;
        // Return new root
        return y;
    }

    // Get Balance factor of node N
    int GetBalance(Leaf* N) {
        if (N == nullptr)
            return 0;
        return Height(N->left) - Height(N->right);
    }
};