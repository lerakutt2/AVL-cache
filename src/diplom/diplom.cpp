#ifdef _WIN32
#include "diplom/GetTimeOfDay.h"
#else
#include <sys/time.h> // gettimeofday
#endif
#include "diplom/TreeList.h"        // шаблонный класс дерева
#include "diplom/Functions.h"   // прототипы функций
#include "diplom/Utils.h"

#include <ctime>    // time
#include <iomanip>  // setprecision
#include <cstring>
#include <iostream>
#include <quadmath.h>
#include <vector> 

using namespace std;

std::vector<__float128> P;
std::vector<__float128> GAMMA;
std::vector<__float128> ALPHA;
std::vector<__float128> GAM;
std::vector<int> L;
std::vector<__float128> ggResult;

//void Calculate(int numberOfLines) {
//    clock_t startTime, endTime;
//    struct timeval timeStart, timeEnd;
//    gettimeofday(&timeStart, NULL);
//    startTime = clock();
//    // start of code to measure
//
//    char buf[128];
//    __float128 result;
//
//    for (int idx = 0; idx < numberOfLines; ++idx) {
//        result = GG(L[idx], ALPHA[idx], GAMMA[idx], P[idx], GAM[idx]);
//        quadmath_snprintf(buf, sizeof buf, "%*.34Qf", 10, result);
//        cout << "Значение функции: " << buf << endl;
//    }
//
//    // end of code to measure
//    endTime = clock();
//    gettimeofday(&timeEnd, NULL);
//    cout << setprecision(14) << "Calculation time (s) = " << ((static_cast<double>(endTime - startTime)) / CLOCKS_PER_SEC) << endl;
//    cout << setprecision(14) << "Wallclock time   (s) = " << ((static_cast<double>(timeEnd.tv_sec - timeStart.tv_sec + (timeEnd.tv_usec - timeStart.tv_usec) * 0.000001))) << endl;
//}
//
//
//void CalculateWithTree(int numberOfLines) {
//    clock_t startTime, endTime;
//    struct timeval timeStart, timeEnd;
//    gettimeofday(&timeStart, NULL);
//
//    __float128 result;
//
////    TreeList<__float128, int, __float128, __float128, __float128, __float128> GGTree(GG); // ускорение в зависимости от количества повторов (один и тот же кусочек данных снова)
//    // start of code to measure
//    TreeList GGtree(GG); // например второй аргумент это количество входных.
//
//    startTime = clock();
//    for (int idx = 0; idx < numberOfLines; ++idx) {
//        __float128* data = new __float128[5]{
//            static_cast<__float128>(L[idx]),
//            ALPHA[idx],
//            GAMMA[idx],
//            P[idx],
//            GAM[idx]
//        };
//        //GGTree.GetValue(data, result);
//        
//        cout << "Параметры функции:" << L[idx] << " ";
//        printFloat128(4, ALPHA[idx], GAMMA[idx], P[idx], GAM[idx]);
//
//        //cout << "correct ";
//        //printFloat128(ggResult[idx]);
//
//        result = GGtree.GetValue(L[idx], ALPHA[idx], GAMMA[idx], P[idx], GAM[idx]);
//        cout << "tree: ";
//        printFloat128(result);
//        cout<< endl;
//        //cout << compareFloat128(ggResult[idx], result);
//        delete[] data;
//    }
//
//    // end of code to measure
//    endTime = clock();
//    gettimeofday(&timeEnd, NULL);
//    cout << setprecision(14) << "Calculation time (s) = " << ((static_cast<double>(endTime - startTime)) / CLOCKS_PER_SEC) << endl;
//    cout << setprecision(14) << "Wallclock time   (s) = " << ((static_cast<double>(timeEnd.tv_sec - timeStart.tv_sec + (timeEnd.tv_usec - timeStart.tv_usec) * 0.000001))) << endl;
//}

void CalculateSum() {
    // Создаем 10 пар целочисленных значений
    std::vector<std::pair<int, double>> pairs = {
        {1, 2}, {1, 2}, {5, 6}, {7, 8}, {9, 10},
        {11, 12}, {13, 14}, {15, 16}, {5, 6}, {19, 20}
    };

    TreeList tree(Sum); // например второй аргумент это количество входных.
    //TreeList<int, int, int> tree(Sum);
    // Вызываем функцию для каждой пары
    for (size_t i = 0; i < pairs.size(); ++i) { 
        double result = tree.GetValue(pairs[i].first, pairs[i].second);
        std::cout << "Pair " << i + 1 << ": " << pairs[i].first << " + "
            << pairs[i].second << " = " << result << std::endl;
    }
}


int main(void) {
    //int numberOfLines[] = { 10};
    //for (int i = 0; i < size(numberOfLines); i++) {
    //    cout << "Количество строк: " << numberOfLines[i] << endl;
    //    P.clear();
    //    GAMMA.clear();
    //    ALPHA.clear();
    //    GAM.clear();
    //    L.clear();
    //    ggResult.clear();
    //    LoadData(numberOfLines[i], P, GAMMA, ALPHA, GAM, L, ggResult);

    //    cout << "Function call:" << endl;
    //    Calculate(numberOfLines[i]);
    //    cout << "Using Tree" << endl;
    //    CalculateWithTree(numberOfLines[i]);
    //}
    CalculateSum();
    TreeList tree(Sum);
    return 0;
}
