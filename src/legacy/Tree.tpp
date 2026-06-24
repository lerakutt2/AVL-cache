// Tree.tpp - только определения методов, без повторного объявления класса

#include <cstdlib>

// Конструктор
template<typename InputType, typename ReturnType>
Tree<InputType, ReturnType>::Tree() {
    headSinCos.left = nullptr;
    headSinCos.right = nullptr;
    headSinCos.next = nullptr;
    headSinCos.value = nullptr;
    headSinCos.height = 1;
}

// Деструктор
template<typename InputType, typename ReturnType>
Tree<InputType, ReturnType>::~Tree() {
    if (headSinCos.next != nullptr) {
        CleanTree(headSinCos.next);
    }
}

template<typename InputType, typename ReturnType>
void Tree<InputType, ReturnType>::GetValue(InputType* data, ReturnType& retData) {
    
    Leaf<InputType>* lastHead = nullptr;  
    void* result = GetValue(0, &headSinCos, data, &lastHead);
    retData = *static_cast<ReturnType*>(result);
}

template<typename InputType, typename ReturnType>
void* Tree<InputType, ReturnType>::GetValue(int actLevel, Leaf<InputType>* head, 
                                            InputType* data, Leaf<InputType>** lastLevelHead) {
    Leaf<InputType>* actHead = head;
    Leaf<InputType>* actNode;
    
    if (actLevel == 0)
        (*lastLevelHead) = head;
    else
        (*lastLevelHead) = nullptr;
    
    InputType actPar;
    bool nodeFound;
    
    for (; actLevel > -1; --actLevel) {
        actPar = data[actLevel];
        actNode = actHead->next;
        nodeFound = false;
        
        while (1) {
            if (actNode == nullptr)
                break;
            if (actPar == actNode->argument) {
                nodeFound = true;
                break;
            }
            else if (actPar < actNode->argument)
                actNode = actNode->left;
            else
                actNode = actNode->right;
        }
        
        if (nodeFound) {
            if (actLevel == 0)
                return actNode->value;
            else if (actLevel == 1) {
                (*lastLevelHead) = actNode;
                actHead = actNode;
            }
            else
                actHead = actNode;
        }
        else {
            Leaf<InputType>* result;
            actHead->next = InsertNode(actHead->next, data, actLevel, result, lastLevelHead);
            return result->value;
        }
    }
    
    exit(EXIT_FAILURE);
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
Leaf<InputType>* Tree<InputType, ReturnType>::NewNode(InputType* data, const int& actLevel,
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
    
    // пока только GG
     ReturnType* tmpVal = new ReturnType;
     cout << "Вызываем GG" << endl;
     *tmpVal = GG((int&)data[0], data[1], data[2], data[3], data[4]);
     lastNode->value = static_cast<void*>(tmpVal);
     value = lastNode;
    
    return node;
}

// InsertNode
template<typename InputType, typename ReturnType>
Leaf<InputType>* Tree<InputType, ReturnType>::InsertNode(Leaf<InputType>* node, InputType* data,
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
int Tree<InputType, ReturnType>::Height(Leaf<InputType>* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

// Max
template<typename InputType, typename ReturnType>
int Tree<InputType, ReturnType>::Max(const int& a, const int& b) {
    return (a > b) ? a : b;
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