#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>
#ifdef _WIN32
#include "diplom/GetTimeOfDay.h"
#else
#include <sys/time.h> // gettimeofday
#endif

#include "AVL-cache/TreeList.h" // шаблонный класс дерева
using namespace std;

double func(double x, double y) {
  if (x == 0.0)
    return 0.0;

  double result;
  result = std::sin(x);
  result *= std::log(y + 1.0);
  result = std::pow(result, x / y);
  result *= std::pow(x, 1.0 / y);
  for (int i = 0; i < 1000; i++) {
    result += sqrt(rand());
  }
  return result;
}

double random_double(double a, double b) {
  return a + (b - a) * std::rand() / double(RAND_MAX);
}

int func_repeat(const int Ncalls, const double repeat_percent) {
  const double pi = std::acos(-1.0);

  const double xmin = 0.0;
  const double xmax = pi / 2.0;

  const double ymin = pi / 2.0;
  const double ymax = pi;

  int Nrepeat = int(Ncalls * repeat_percent / 100.0);
  int Nunique = Ncalls - Nrepeat;

  std::vector<double> x_vals(Nunique);
  std::vector<double> y_vals(Nunique);

  double sum = 0.0;

  clock_t startTime, endTime;
  struct timeval timeStart, timeEnd;
  gettimeofday(&timeStart, NULL);

  startTime = clock();

  // First create exactly Nunique unique argument pairs.
  for (int i = 0; i < Nunique; ++i) {
    x_vals[i] = random_double(xmin, xmax);
    y_vals[i] = random_double(ymin, ymax);

    sum += func(x_vals[i], y_vals[i]);
  }

  // Then make exactly Nrepeat repeated calls.
  for (int i = 0; i < Nrepeat; ++i) {
    int k = std::rand() % Nunique;
    sum += func(x_vals[k], y_vals[k]);
  }

  endTime = clock();
  gettimeofday(&timeEnd, NULL);
  cout << setprecision(15)
       << ((static_cast<double>(endTime - startTime)) / CLOCKS_PER_SEC / Ncalls)
       << endl;
  cout << setprecision(15)
       << ((static_cast<double>(timeEnd.tv_sec - timeStart.tv_sec +
                                (timeEnd.tv_usec - timeStart.tv_usec) *
                                    0.000001)) /
           Ncalls)
       << endl;
  // cout << setprecision(15) << ((static_cast<double>(endTime - startTime)) /
  // CLOCKS_PER_SEC) << endl; cout << setprecision(15)<<
  // ((static_cast<double>(timeEnd.tv_sec - timeStart.tv_sec + (timeEnd.tv_usec
  // - timeStart.tv_usec) * 0.000001))) << endl;

  return 0;
}

int func_repeat_tree(const int Ncalls, const double repeat_percent) {
  const double pi = std::acos(-1.0);

  const double xmin = 0.0;
  const double xmax = pi / 2.0;

  const double ymin = pi / 2.0;
  const double ymax = pi;

  int Nrepeat = int(Ncalls * repeat_percent / 100.0);
  int Nunique = Ncalls - Nrepeat;

  std::vector<double> x_vals(Nunique);
  std::vector<double> y_vals(Nunique);

  double sum = 0.0;
  TreeList tree(func);

  clock_t startTime, endTime;
  struct timeval timeStart, timeEnd;
  gettimeofday(&timeStart, NULL);

  startTime = clock();
  // First create exactly Nunique unique argument pairs.
  for (int i = 0; i < Nunique; ++i) {
    x_vals[i] = random_double(xmin, xmax);
    y_vals[i] = random_double(ymin, ymax);

    sum += tree.GetValue(x_vals[i], y_vals[i]);
  }

  // Then make exactly Nrepeat repeated calls.
  for (int i = 0; i < Nrepeat; ++i) {
    int k = std::rand() % Nunique;
    sum += tree.GetValue(x_vals[k], y_vals[k]);
  }

  endTime = clock();
  gettimeofday(&timeEnd, NULL);
  cout << setprecision(15)
       << ((static_cast<double>(endTime - startTime)) / CLOCKS_PER_SEC / Ncalls)
       << endl;
  cout << setprecision(15)
       << ((static_cast<double>(timeEnd.tv_sec - timeStart.tv_sec +
                                (timeEnd.tv_usec - timeStart.tv_usec) *
                                    0.000001)) /
           Ncalls)
       << endl;
  // cout << setprecision(15) << ((static_cast<double>(endTime - startTime)) /
  // CLOCKS_PER_SEC) << endl; cout << setprecision(15) <<
  // ((static_cast<double>(timeEnd.tv_sec - timeStart.tv_sec + (timeEnd.tv_usec
  // - timeStart.tv_usec) * 0.000001))) << endl;

  return 0;
}
