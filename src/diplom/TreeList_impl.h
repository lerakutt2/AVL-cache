#pragma once
#include <cstdlib>
#include <iostream>
#include <functional>
#include <tuple>

// Constructor
template<typename ReturnType, typename... InputTypes>
TreeList<ReturnType, InputTypes...>::TreeList() {
    head.left = nullptr;
    head.right = nullptr;
    head.next = nullptr;
    head.value = nullptr;
    head.height = 1;
}

// Destructor
template<typename ReturnType, typename... InputTypes>
TreeList<ReturnType, InputTypes...>::~TreeList() {
    if (head.left) CleanTreeList(head.left);
    if (head.right) CleanTreeList(head.right);
    if (head.next) CleanTreeList(head.next);
}

// public GetValue
template<typename ReturnType, typename... InputTypes>
void TreeList<ReturnType, InputTypes...>::GetValue(InputType* data, ReturnType& retData) {
    Leaf<InputType>* lastHead = nullptr;
    void* result = GetValue(5, &head, data, &lastHead);
    retData = *static_cast<ReturnType*>(result);
}

// private GetValue
template<typename ReturnType, typename... InputTypes>
ReturnType TreeList<ReturnType, InputTypes...>::GetValue(InputTypes... data) {
    int actLevel = sizeof...(data);
    auto params = std::make_tuple(data);
    //InputType actPar;
    bool nodeFound; // I have found node with given properties (parameter value)
    for (; actLevel >= 0; --actLevel) { // cycle traversing all parameters (all Trees)
        actPar = params.get(actLevel);
        tree.GetValue(actPar);
        // если не находится, то вернет уровень и ссылку на недостающий узел? 
        // может тогда здесь будет метод который создаст новые деревья выше уровнем?
        // вернет ссылку на след. дерево или значение
        // может быть этот метод будет возвращать ссылку на узел и если этот узел пустой то здесь будет вызываться метод создания?
        
        // будет в дереве:
        // while (1) {               // cycle across all leaves on a given TreeList
        //    if (actNode == nullptr) // I have not found given node, need to add
        //        break;
        //    if (actPar == actNode->argument) {
        //        nodeFound = true;

        //        //char buf[128];
        //        //quadmath_snprintf(buf, sizeof(buf), "%.30Qf", actPar);
        //        //cout << "node found! data[" << actLevel << "]=" << buf << endl;
        //        break;
        //    }
        //    else if (actPar < actNode->argument)
        //        actNode = actNode->left;
        //    else
        //        actNode = actNode->right;
        //}                // cycle across all leaves
        //if (nodeFound) { // I have found requested node, either return result or go
        //    // one level further
        //    if (actLevel == 0)
        //        return actNode->value;
        //    else if (actLevel == 1) { // special case, storing pointer to 0-th level
        //        // head for reuse
        //        (*lastLevelHead) = actNode;
        //        actHead = actNode;
        //    }
        //    else
        //        actHead = actNode;
        //}
        //else { // I have not found requested node, add it and return result

        //    Leaf<InputType>* result;
        //    actHead->next = InsertNode(actHead->next, data, actLevel, result,
        //        lastLevelHead);
        //    return result->value;
        //}
    }
    // cycle across all parameters
    throw std::runtime_error("TreeList::GetValue: unexpected state");
}

// CleanTreeList
template<typename InputType, typename ReturnType>
void TreeList<InputType, ReturnType>::CleanTreeList(Leaf<InputType>* leaf) {
    if (leaf == nullptr)
        return;

    Leaf<InputType>* leftChild = leaf->left;
    Leaf<InputType>* rightChild = leaf->right;
    Leaf<InputType>* nextChild = leaf->next;

    CleanTreeList(leftChild);
    CleanTreeList(rightChild);
    CleanTreeList(nextChild);

    if (leaf->value != nullptr) {
        delete static_cast<ReturnType*>(leaf->value);
    }

    delete leaf;
}

// NewNode
template<typename InputType, typename ReturnType>
Leaf<InputType>* TreeList<InputType, ReturnType>::NewNode(InputType* data, const int& actLevel,
    Leaf<InputType>*& value,
    Leaf<InputType>** lastLevelHead) {
    Leaf<InputType>* node = new Leaf<InputType>(data[actLevel]);
    Leaf<InputType>* lastNode = node;

    if (actLevel == 1)
        (*lastLevelHead) = node;

    for (int idx = actLevel - 1; idx >= 0; --idx) {
        lastNode->next = new Leaf<InputType>(data[idx]);
        lastNode = lastNode->next;
        if (idx == 1)
            (*lastLevelHead) = lastNode;
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
Leaf<InputType>* TreeList<InputType, ReturnType>::InsertNode(Leaf<InputType>* node, InputType* data,
    const int& actLevel, Leaf<InputType>*& value,
    Leaf<InputType>** lastLevelHead) {
    if (node == nullptr)
        return NewNode(data, actLevel, value, lastLevelHead);

    if (data[actLevel] > node->argument)
        node->right = InsertNode(node->right, data, actLevel, value, lastLevelHead);
    else
        node->left = InsertNode(node->left, data, actLevel, value, lastLevelHead);

    node->height = 1 + Max(Height(node->left), Height(node->right));

    int balance = GetBalance(node);

    if (balance > 1 && data[actLevel] < node->left->argument)
        return RightRotate(node);

    if (balance < -1 && data[actLevel] > node->right->argument)
        return LeftRotate(node);

    if (balance > 1 && data[actLevel] > node->left->argument) {
        node->left = LeftRotate(node->left);
        return RightRotate(node);
    }

    if (balance < -1 && data[actLevel] < node->right->argument) {
        node->right = RightRotate(node->right);
        return LeftRotate(node);
    }

    return node;
}

// Height
template<typename InputType, typename ReturnType>
int TreeList<InputType, ReturnType>::Height(Leaf<InputType>* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

// RightRotate
template<typename InputType, typename ReturnType>
Leaf<InputType>* TreeList<InputType, ReturnType>::RightRotate(Leaf<InputType>* y) {
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
Leaf<InputType>* TreeList<InputType, ReturnType>::LeftRotate(Leaf<InputType>* x) {
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
int TreeList<InputType, ReturnType>::GetBalance(Leaf<InputType>* N) {
    if (N == nullptr)
        return 0;
    return Height(N->left) - Height(N->right);
}