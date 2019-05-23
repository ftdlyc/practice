#include <stdio.h>

#include <algorithm>
#include <chrono>
#include <random>
#include <vector>
using namespace std::chrono;

#include "head_sort.hpp"
#include "insertion_sort.hpp"
#include "merge_sort.hpp"
#include "quick_sort.hpp"

int main() {
  int len = 1000000;
  std::vector<int> v(len);
  for(int i = 0; i < v.size(); ++i) v[i] = i;

  std::srand(std::time(nullptr));
  std::random_shuffle(v.begin(), v.end());
  std::vector<int> v2(v);
  std::vector<int> v3(v);
  std::vector<int> v4(v);

  {
    auto t1 = system_clock::now();
    head_sort(v.begin(), v.end());
    auto t2 = system_clock::now();
    printf("head sort took: %f ms\n", duration_cast<microseconds>((t2 - t1)).count() / 1000.f);
  }
  {
    auto t1 = system_clock::now();
    quick_sort(v2.begin(), v2.end());
    auto t2 = system_clock::now();
    printf("quick sort took: %f ms\n", duration_cast<microseconds>((t2 - t1)).count() / 1000.f);
  }
  {
    auto t1 = system_clock::now();
    merge_sort(v3.begin(), v3.end());
    auto t2 = system_clock::now();
    printf("merge sort took: %f ms\n", duration_cast<microseconds>((t2 - t1)).count() / 1000.f);
  }
  {
    auto t1 = system_clock::now();
    std::sort(v4.begin(), v4.end());
    auto t2 = system_clock::now();
    printf("stl sort took: %f ms\n", duration_cast<microseconds>((t2 - t1)).count() / 1000.f);
  }

  return 0;
}