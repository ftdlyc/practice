#ifndef HEAD_SORT_HPP
#define HEAD_SORT_HPP

#include <iterator>

template<typename _RandomAccessIterator, typename _Distance, typename _T>
inline void push_head_impl(_RandomAccessIterator first, _Distance hole_index, _Distance top_index, _T value) {
  while(hole_index > top_index) {
    _Distance parent_index = (hole_index - 1) / 2;
    _T parent_value        = *(first + parent_index);

    if(parent_value < value) {
      *(first + hole_index) = parent_value;
      hole_index            = parent_index;
    } else {
      break;
    }
  }
  *(first + hole_index) = value;
}

template<typename _RandomAccessIterator, typename _Distance, typename _T>
inline void push_head_aux(_RandomAccessIterator first, _RandomAccessIterator last, _Distance*, _T*) {
  push_head_impl(first, _Distance(last - first - 1), _Distance(0), _T(*(last - 1)));
}

template<typename _RandomAccessIterator>
inline void push_head(_RandomAccessIterator first, _RandomAccessIterator last) {
  push_head_aux(first, last,
                static_cast<typename std::iterator_traits<_RandomAccessIterator>::difference_type*>(0),
                static_cast<typename std::iterator_traits<_RandomAccessIterator>::value_type*>(0));
}

template<typename _RandomAccessIterator, typename _Distance, typename _T>
inline void adjust_head(_RandomAccessIterator first, _Distance hole_index, _Distance len, _T value) {
  _Distance top_index   = hole_index;
  _Distance child_index = hole_index * 2 + 2;
  while(child_index < len) {
    if(*(first + child_index) < *(first + (child_index - 1))) --child_index;
    *(first + hole_index) = *(first + child_index);
    hole_index            = child_index;
    child_index           = child_index * 2 + 2;
  }
  if(child_index == len) {
    *(first + hole_index) = *(first + (child_index - 1));
    hole_index            = child_index - 1;
  }

  push_head_impl(first, hole_index, top_index, value);
}

template<typename _RandomAccessIterator, typename _Distance, typename _T>
inline void pop_head_impl(_RandomAccessIterator first, _RandomAccessIterator last, _RandomAccessIterator result, _T value, _Distance*) {
  *result = *first;
  adjust_head(first, _Distance(0), _Distance(last - first), value);
}

template<typename _RandomAccessIterator, typename _T>
inline void pop_head_aux(_RandomAccessIterator first, _RandomAccessIterator last, _T*) {
  pop_head_impl(first, last - 1, last - 1, _T(*(last - 1)),
                static_cast<typename std::iterator_traits<_RandomAccessIterator>::difference_type*>(0));
}

template<typename _RandomAccessIterator>
inline void pop_head(_RandomAccessIterator first, _RandomAccessIterator last) {
  pop_head_aux(first, last,
               static_cast<typename std::iterator_traits<_RandomAccessIterator>::value_type*>(0));
}

template<typename _RandomAccessIterator, typename _Distance, typename _T>
inline void make_head_impl(_RandomAccessIterator first, _RandomAccessIterator last, _Distance*, _T*) {
  _Distance len = last - first;
  if(len < 2) return;

  _Distance parent = (len - 2) / 2;
  for(; parent >= 0; --parent) {
    adjust_head(first, parent, len, _T(*(first + parent)));
  }
}

template<typename _RandomAccessIterator>
inline void make_head(_RandomAccessIterator first, _RandomAccessIterator last) {
  make_head_impl(first, last,
                 static_cast<typename std::iterator_traits<_RandomAccessIterator>::difference_type*>(0),
                 static_cast<typename std::iterator_traits<_RandomAccessIterator>::value_type*>(0));
}

template<typename _RandomAccessIterator>
inline void sort_head(_RandomAccessIterator first, _RandomAccessIterator last) {
  while(last - first > 1) {
    pop_head(first, last);
    --last;
  }
}

template<typename _RandomAccessIterator>
void head_sort(_RandomAccessIterator first, _RandomAccessIterator last) {
  make_head(first, last);
  sort_head(first, last);
}

#endif //HEAD_SORT_HPP
