#include "Test/Utils.h"

#include <climits>
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <quadmath.h>

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

void LoadData(int numberOfLines, std::vector<__float128> &P,
              std::vector<__float128> &GAMMA, std::vector<__float128> &ALPHA,
              std::vector<__float128> &GAM, std::vector<int> &L,
              std::vector<__float128> &ggResult) {

  FILE *fpIn = fopen("ggParameters.txt", "r");
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
    cerr << "File is empty or error reading header" << endl;
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
    if (++linesRead >= numberOfLines)
      break;
  }

  fclose(fpIn);
}