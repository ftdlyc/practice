#ifndef QUICK_SORT_HPP
#define QUICK_SORT_HPP

#include <iterator>

#include "insertion_sort.hpp"

const int k_quick_sort_threshold = 16;

template<typename _T>
_T median(_T a, _T b, _T c) {
  if(a > b) {
    if(a > c) {
      if(b > c) return b;
      return c;
    }
    return a;
  } else {
    if(a < c) {
      if(b > c) return c;
      return b;
    }
    return a;
  }
}

template<typename _RandomAccessIterator, typename _T>
_RandomAccessIterator unguarded_partition(_RandomAccessIterator first, _RandomAccessIterator last, _T pivot) {
  while(first < last) {
    while(*first < pivot) ++first;
    --last;
    while(*last > pivot) --last;

    if(!(first < last)) return first;
    std::iter_swap(first, last);
    ++first;
  }
}

template<typename _RandomAccessIterator>
void quick_sort_loop_aux(_RandomAccessIterator first, _RandomAccessIterator last);

template<typename _RandomAccessIterator, typename _T>
void quick_sort_loop(_RandomAccessIterator first, _RandomAccessIterator last, _T*) {
  if(last - first > k_quick_sort_threshold) {
    _RandomAccessIterator cut =
        unguarded_partition(first, last, median(*first, *(last - 1), *(first + (last - first) / 2)));

    quick_sort_loop_aux(first, cut);
    quick_sort_loop_aux(cut, last);
  }
}

template<typename _RandomAccessIterator>
void quick_sort_loop_aux(_RandomAccessIterator first, _RandomAccessIterator last) {
  quick_sort_loop(first, last,
                  static_cast<typename std::iterator_traits<_RandomAccessIterator>::value_type*>(0));
}

template<typename _RandomAccessIterator>
void quick_sort(_RandomAccessIterator first, _RandomAccessIterator last) {
  quick_sort_loop_aux(first, last);
  insertion_sort(first, last);
}

#endif //QUICK_SORT_HPP
