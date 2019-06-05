#include <stdio.h>

template<int... N>
struct index_seq {};

template<int N, int... M>
struct make_index_seq : public make_index_seq<N - 1, N - 1, M...> {
};

template<int... M>
struct make_index_seq<0, M...> : public index_seq<M...> {
};

template<typename... T>
struct make_arg_index_seq : public make_index_seq<sizeof...(T)> {
};

template<int N, typename T>
void print_arg_index_seq(index_seq<N> in, T t) {
  printf("%d-%d\n", N, t);
}

template<int N, int... M, typename T, typename... Ts>
void print_arg_index_seq(index_seq<N, M...> in, T t, Ts... ts) {
  printf("%d-%d\n", N, t);
  print_arg_index_seq(index_seq<M...>(), ts...);
}

template<typename... T>
void run(T... t) {
  print_arg_index_seq(make_arg_index_seq<T...>(), t...);
}

int main() {
  int m[3] = {10, 11, 12};
  run<int, int, int>(m[0], m[1], m[2]);

  return 0;
}
