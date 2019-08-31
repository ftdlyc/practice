#include <chrono>
#include <iostream>
#include <random>
using namespace std::chrono;
using std::default_random_engine;
using std::uniform_int_distribution;

template<typename T, size_t N>
class SkipList {
public:
  static const size_t nlayers = N;

  using ElemType = T;

  typedef struct Node {
    ElemType data;
    Node* next[nlayers];
    Node* last[nlayers];

    Node() {
      for(int i = 0; i < nlayers; ++i) {
        next[i] = NULL;
        last[i] = NULL;
      }
    }

    Node(const ElemType& val)
        : data(val) {
    }
  } Node;

  SkipList()
      : root(new Node)
      , ncounts(0) {
  }

  ~SkipList() {
    Node* ptr = root;
    while(ptr->next[nlayers - 1] != NULL) {
      ptr = ptr->next[nlayers - 1];
      delete ptr->last[nlayers - 1];
    }
    delete ptr;
  }

  Node* find(const ElemType& val) {
    Node* ptr = root;
    for(int i = 0; i < nlayers; ++i) {
      while(ptr->next[i] != NULL && val > ptr->next[i]->data) {
        ptr = ptr->next[i];
      }
      if(ptr->next[i] != NULL && val == ptr->next[i]->data) return ptr->next[i];
    }
    return NULL;
  }

  Node* insert(const ElemType& val) {
    size_t layer = random_layer();

    Node* ptr = root;
    Node* ptr_last[nlayers];
    Node* ptr_next[nlayers];
    for(int i = 0; i < nlayers; ++i) {
      while(ptr->next[i] != NULL && val > ptr->next[i]->data) {
        ptr = ptr->next[i];
      }
      ptr_last[i] = ptr;
      ptr_next[i] = ptr->next[i];
    }

    Node* new_node = new Node(val);
    for(int i = 0; i < layer; ++i) {
      new_node->last[i] = NULL;
      new_node->next[i] = NULL;
    }
    for(int i = layer; i < nlayers; ++i) {
      new_node->last[i] = ptr_last[i];
      new_node->next[i] = ptr_next[i];
      ptr_last[i]->next[i] = new_node;
      if(ptr_next[i] != NULL) ptr_next[i]->last[i] = new_node;
    }

    ++ncounts;
    return new_node;
  }

  void remove(Node* node) {
    for(int i = 0; i < nlayers; ++i) {
      if(node->last[i] == NULL) continue;
      node->last[i]->next[i] = node->next[i];
      if(node->next[i] != NULL) node->next[i]->last[i] = node->last[i];
    }
    delete node;
    --ncounts;
  }

  inline size_t size() {
    return ncounts;
  }

  inline size_t layer() {
    return nlayers;
  }

  void print() {
    for(int i = 0; i < nlayers; ++i) {
      Node* ptr = root;
      std::cout << "root-->";
      while(ptr->next[i] != NULL) {
        std::cout << ptr->next[i]->data << "-->";
        ptr = ptr->next[i];
      }
      std::cout << "NULL\n";
    }
    std::cout << "\n";
  }

private:
  int random_layer() {
    static default_random_engine random_engine(system_clock::now().time_since_epoch().count());
    static uniform_int_distribution<size_t> distribution(0, nlayers - 1);
    return distribution(random_engine);
  }

  Node* root;
  size_t ncounts;
};

int main() {
  SkipList<int, 3> sl;
  for(int i = 0; i < 20; ++i) {
    sl.insert(i);
  }
  sl.print();
  for(int i = 0; i < 20; i += 2) {
    auto node = sl.find(i);
    sl.remove(node);
  }
  sl.print();
  return 0;
}
