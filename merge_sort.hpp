#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP

#include <iterator>
#include <vector>

#include "insertion_sort.hpp"

const int k_merge_sort_threshold = 32;

template<typename _RandomAccessIterator>
void merge_sort_loop_aux(_RandomAccessIterator first, _RandomAccessIterator last);

template<typename _RandomAccessIterator, typename _T>
void merge_sort_loop(_RandomAccessIterator first, _RandomAccessIterator last, _T*) {
  if(last - first > k_merge_sort_threshold) {
    _RandomAccessIterator mid = first + (last - first) / 2;
    merge_sort_loop_aux(first, mid);
    merge_sort_loop_aux(mid, last);

    std::vector<_T> tmp(last - first);
    _RandomAccessIterator i = first;
    _RandomAccessIterator j = mid;
    _RandomAccessIterator k = tmp.begin();
    for(; i != mid && j != last; ++k) {
      if(*i < *j) {
        *k = *i;
        ++i;
      } else {
        *k = *j;
        ++j;
      }
    }
    if(i != mid) {
      std::copy(i, mid, last - (mid - i));
    }

    std::copy(tmp.begin(), k, first);
  }
  insertion_sort(first, last);
}

template<typename _RandomAccessIterator>
void merge_sort_loop_aux(_RandomAccessIterator first, _RandomAccessIterator last) {
  merge_sort_loop(first, last,
                  static_cast<typename std::iterator_traits<_RandomAccessIterator>::value_type*>(0));
}

template<typename _RandomAccessIterator>
void merge_sort(_RandomAccessIterator first, _RandomAccessIterator last) {
  if(first == last || last - first == 1) return;
  merge_sort_loop_aux(first, last);
}

#endif //MERGE_SORT_HPP
