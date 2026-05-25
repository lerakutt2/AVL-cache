#pragma once
#include <quadmath.h>
#include <vector>

void printFloat128(int count, ...);

void LoadData(int numberOfLines,
    std::vector<__float128>& P,
    std::vector<__float128>& GAMMA,
    std::vector<__float128>& ALPHA,
    std::vector<__float128>& GAM,
    std::vector<int>& L,
    std::vector<__float128>& ggResult);