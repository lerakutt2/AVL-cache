#include "diplom/Utils.h"

#include <climits>
#include <iostream>    
#include <cstdio>
#include <quadmath.h>
#include <cstdarg>

using namespace std;

void printFloat128(int count, ...) {
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        __float128 value = va_arg(args, __float128);
        char buf[128];
        quadmath_snprintf(buf, sizeof(buf), "%.34Qf", value);
        cout << buf << " ";
    }

    va_end(args);
    cout << endl;
}

void LoadData(int numberOfLines,
    std::vector<__float128>& P,
    std::vector<__float128>& GAMMA,
    std::vector<__float128>& ALPHA,
    std::vector<__float128>& GAM,
    std::vector<int>& L,
    std::vector<__float128>& ggResult) {

    FILE* fpIn = fopen("../ggParameters.txt/ggParameters.txt", "r");
    if (fpIn == nullptr) {
        cerr << "Error opening input file\n";
        exit(1);
    }

    char line[1000];
    char cP[50], cGamma[50], cAlpha[50], cGam[50], cResult[50];
    int iL;
    int linesRead = 0;
    __float128 qs;

    // Читаем заголовок (первую строку)
    if (fgets(line, sizeof(line), fpIn) == nullptr) {
        cerr << "Файл пуст или ошибка чтения заголовка" << endl;
        fclose(fpIn);
        return;
    }

    int lineCount = 1;
    while (fgets(line, sizeof(line), fpIn) != nullptr) {
        sscanf(line, "%s%s%s%s%d%s", cP, cGamma, cAlpha, cGam, &iL, cResult);

        qs = strtoflt128(cP, NULL);
        P.push_back(qs);

        qs = strtoflt128(cGamma, NULL);
        GAMMA.push_back(qs);

        qs = strtoflt128(cAlpha, NULL);
        ALPHA.push_back(qs);

        qs = strtoflt128(cGam, NULL);
        GAM.push_back(qs);

        L.push_back(iL);

        qs = strtoflt128(cResult, NULL);

        ggResult.push_back(qs);

        lineCount++;
        if (++linesRead >= numberOfLines) break;
    }

    fclose(fpIn);
}

// Убираем значение по умолчанию из определения!
bool compareFloat128(__float128 a, __float128 b, __float128 epsilon) {
    __float128 diff = a - b;
    if (diff < 0) diff = -diff;
    return diff <= epsilon;
}

int safeFloat128ToInt(__float128 val) {
    // Проверка на переполнение
    if (val > INT_MAX || val < INT_MIN) {
        cerr << "Ошибка: значение вне диапазона int" << endl;
        return 0;
    }

    // Проверка на дробную часть
    __float128 intpart;
    if (modfq(val, &intpart) != 0.0Q) {
        cerr << "Предупреждение: дробная часть будет отброшена" << endl;
    }

    return static_cast<int>(val);
}