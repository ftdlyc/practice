#ifndef INSERTION_SORT_HPP
#define INSERTION_SORT_HPP

#include <algorithm>
#include <iterator>

template<typename _RandomAccessIterator, typename _T>
inline void unguarded_linear_insert(_RandomAccessIterator last, _T value) {
  _RandomAccessIterator next = last - 1;
  while(value < *next) {
    *last = *next;
    last  = next;
    --next;
  }
  *last = value;
}

template<typename _RandomAccessIterator, typename _T>
inline void linear_insert(_RandomAccessIterator first, _RandomAccessIterator last, _T*) {
  _T value = *last;
  if(value < *first) {
    std::copy_backward(first, last, last + 1);
    *first = value;
  } else {
    unguarded_linear_insert(last, value);
  }
}

template<typename _RandomAccessIterator>
inline void insertion_sort(_RandomAccessIterator first, _RandomAccessIterator last) {
  if(first == last) return;
  for(_RandomAccessIterator i = first + 1; i != last; ++i) {
    linear_insert(first, i,
                  static_cast<typename std::iterator_traits<_RandomAccessIterator>::value_type*>(0));
  }
}

#endif //INSERTION_SORT_HPP
