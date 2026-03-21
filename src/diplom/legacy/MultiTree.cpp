#include "diplom/MultiTree.h"

#include <cmath>
#include <cstring>
#include <iostream>

// #define GET_STATISTICS

#ifdef GET_STATISTICS
#include <iomanip>
unsigned long int sincos_calc = 0;
unsigned long int sincos_call = 0;

#endif

// sOneTreeLeaf headGG0;
Leaf headSinCos;

/// <summary>
/// Полная рекурсивная очистка дерева
/// </summary>
/// <param name="leaf">Ссылка на лист дерева</param>
/// <param name="type">Тип функции</param>
void CleanTree(Leaf* leaf, const FunctionType& type) {
    if (leaf == nullptr)
        return;
    if (leaf->left != nullptr) {
        CleanTree(leaf->left, type);
        if (leaf->left->value != nullptr) {
            switch (type) {
            case FunctionType::sc:
                delete (sSinCosData*)(leaf->left->value);
                break;
            default:
                delete (double*)(leaf->left->value);
                break;
            }
        }
        delete leaf->left;
    }
    if (leaf->right != nullptr) {
        CleanTree(leaf->right, type);
        if (leaf->right->value != nullptr) {
            switch (type) {
            case FunctionType::sc:
                delete (sSinCosData*)(leaf->right->value);
                break;
            default:
                delete (double*)(leaf->right->value);
                break;
            }
        }
        delete leaf->right;
    }
    if (leaf->next != nullptr) {
        CleanTree(leaf->next, type);
        if (leaf->next->value != nullptr) {
            switch (type) {
            case FunctionType::sc:
                delete (sSinCosData*)(leaf->next->value);
                break;
            default:
                delete (double*)(leaf->next->value);
                break;
            }
        }
        delete leaf->next;
    }
}

#ifdef GET_STATISTICS
void PrintOneParameter(const char* name, const unsigned int& calls,
    const unsigned int& calcs,
    const unsigned int sumCalls = 0) {
    double ratio = 0.0;
    if (sumCalls != 0)
        ratio = 100.0 * static_cast<double>(calcs) / static_cast<double>(sumCalls);
    else if (calls)
        ratio = 100.0 * static_cast<double>(calcs) / static_cast<double>(calls);
    std::cout << name;
    std::cout << std::setw(12) << calls;
    if (calcs) { // if I have not calculated values, don't print them
        std::cout << std::setw(14) << calcs;
        if (ratio < 10)
            fprintf(stdout, " ");
        fprintf(stdout, "     %6.4f\n", ratio);
    }
    else
        std::cout << "\n";
}
#endif

void IntermediateClean(unsigned int msk) {
#ifdef GET_STATISTICS
    std::cerr << "Intermediate stats\n";
    //  std::cerr << "GG0     " << gg0_call << " " << gg0_calc << "\n";
    std::cerr << "SINCOS  " << sincos_call << " " << sincos_calc << "\n";
#endif
    CleanTree(msk);
}

void CleanTree(unsigned int msk) {
    /*
      if (msk & CLEAN_GG0) {
        if (headGG0.next != nullptr) {
          CleanTree(headGG0.next, FunctionType::gg0);
          delete headGG0.next;
          headGG0.next = nullptr;
        }
      }
    */
    if (msk & CLEAN_SINCOS) {


    }
#ifdef GET_STATISTICS
    if (msk == CLEAN_ALL) {
        std::cout << "Function        call         uniqe       ratio\n";
        //    PrintOneParameter("GG0     ", gg0_call, gg0_calc);
        PrintOneParameter("SINCOS  ", sincos_call, sincos_calc);
    }
#endif
}

/*
double GetValueGG0(double *data) {
#ifdef GET_STATISTICS
  ++gg0_call;
#endif
  sOneTreeLeaf *lastHead = nullptr;
  return *(
      (double *)GetValue(2, &headGG0, data, FunctionType::gg0, &lastHead));
}
*/
__float128 GetValueGG(__float128* input, __float128& returnValue) {
#ifdef GET_STATISTICS
    ++gg_call;
#endif
    Leaf* lastHead = nullptr;
    return *(
        (__float128*)GetValue(2, &headSinCos, data, FunctionType::gg, &lastHead));
}

//void GetValueSinCos(double* data, double& retSin, double& retCos) {
//#ifdef GET_STATISTICS
//    ++sincos_call;
//#endif
//    sOneTreeLeaf* lastHead = nullptr;
//    sSinCosData* retData = (sSinCosData*)GetValue(0, &headSinCos, data,
//        FunctionType::sc, &lastHead);
//    retSin = retData->valSin;
//    retCos = retData->valCos;
//}

// A utility function to get maximum of two integers
int Max(const int& a, const int& b) { return (a > b) ? a : b; }

// A utility function to get the height of the tree
int Height(Leaf* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

/* Helper function that allocates a new node with the given key and
   nullptr left and right pointers. */
Leaf* NewNode(double* data, const int& actLevel,
    const FunctionType& type, Leaf*& value,
    Leaf** lastLevelHead) {
    Leaf* node = new Leaf;
    Leaf* lastNode;
    node->argument = data[actLevel];
    lastNode = node;
    if (actLevel == 1)
        (*lastLevelHead) = node;
    for (int idx = actLevel - 1; idx > -1; --idx) {
        lastNode->next = new Leaf;
        lastNode = lastNode->next;
        lastNode->argument = data[idx];
        if (idx == 1)
            (*lastLevelHead) = lastNode;
    }
    switch (type) {
        /*
          case FunctionType::gg0: {
        #ifdef GET_STATISTICS
            ++gg0_calc;
        #endif
            double *tmpVal = new double;
            *tmpVal = GG0(data);
            lastNode->value = (void *)tmpVal;
          } break;
        */
    case FunctionType::sc: {
#ifdef GET_STATISTICS
        ++sincos_calc;
#endif
        sSinCosData* tmpVal = new sSinCosData;
        tmpVal->valSin = std::sin(data[0]);
        tmpVal->valCos = std::cos(data[0]);
        lastNode->value = (void*)tmpVal;
    }
    case FunctionType::gg: {
#ifdef GET_STATISTICS
        ++gg_calc;
#endif
        double* tmpVal = new double;
        *tmpVal = GG(data[0], data[1], data[2], data[3], data[4]);
        lastNode->value = (void*)tmpVal;
    } break;
    }
    value = lastNode;
    return node;
}

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

Leaf* InsertNode(Leaf* node, double* data,
    const int& actLevel, const FunctionType& type,
    Leaf*& value, Leaf** lastLevelHead) {
    /* 1. Perform the normal BST insertion */
    if (node == nullptr)
        return (NewNode(data, actLevel, type, value, lastLevelHead)); // !!!!

    // I'm not testing for equal node since I know that node is not there
    if (data[actLevel] > node->argument)
        node->right =
        InsertNode(node->right, data, actLevel, type, value, lastLevelHead);
    else
        node->left =
        InsertNode(node->left, data, actLevel, type, value, lastLevelHead);

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

/// <summary>
/// Проходит по каждому поддереву и ищет значение. Если не находит,то создает новый лист
/// </summary>
/// <param name="actLevel">Номер поддерева</param>
/// <param name="head">Корень текущего дерева</param>
/// <param name="data">Массив параметров (индекс = порядковый номер параметра в функции = номер поддерева)</param>
/// <param name="type">Тип функции</param>
/// <param name="lastLevelHead">Указатель на корень последнего использованного поддерева</param>
/// <returns></returns>
void* GetValue(int actLevel, Leaf* head, double* data,
    const FunctionType& type, Leaf** lastLevelHead) {
    Leaf* actHead = head;
    Leaf* actNode;
    if (actLevel == 0) // starting from level 0 means, head is on "level 1"
        (*lastLevelHead) = head;
    else
        (*lastLevelHead) = nullptr;
    double actPar;
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
            actHead->next = InsertNode(actHead->next, data, actLevel, type, result,
                lastLevelHead);
            return result->value;
        }
    }          
    cout << "GetValue done" << endl;
    // cycle across all parameters
    exit(EXIT_FAILURE); // I should never come to this point as it means I haven't
    // found/added requested parameters set in(to) the tree
}
