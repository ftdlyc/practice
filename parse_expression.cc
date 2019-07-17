#include <stdlib.h>

#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>

const int k_priority = 5;

template<typename T = float>
class BaseOp {
public:
  using ElemType = T;

  BaseOp()
      : m_n_operands(0)
      , m_priority(0)
      , m_b_number(false){};
  BaseOp(int n_operands, int priority)
      : m_n_operands(n_operands)
      , m_priority(priority)
      , m_b_number(false){};
  BaseOp(ElemType value)
      : m_b_number(true)
      , m_value(value){};

  virtual ElemType operator()(ElemType x, ElemType y) const { return x; }

  int m_n_operands;
  int m_priority;
  int m_b_number;
  ElemType m_value;
};

struct OpMap {
  using OpKey = char;
  using OpPointer = std::shared_ptr<BaseOp<float>>;
  using OpMapType = std::unordered_map<OpKey, OpPointer>;

  static inline OpMapType* get_op_map() {
    static std::unique_ptr<OpMapType> op_map(new OpMapType);
    return op_map.get();
  }

  template<typename Op>
  static inline bool add_op(OpKey key) {
    OpMapType* map = get_op_map();
    if(map->find(key) != map->end()) {
      std::cerr << "Error: " << key << " op has be added\n";
      return false;
    }
    (*map)[key].reset(new Op);
    return true;
  }
};

template<typename OpType>
class OpRegister {
public:
  OpRegister(typename OpMap::OpKey key) {
    OpMap::add_op<OpType>(key);
  }
};

#define REGISTER_OP(key, type, elem_type) static OpRegister<type<elem_type>> _Register##type(key);

template<typename T = float>
class AddOp : public BaseOp<T> {
public:
  using ElemType = typename BaseOp<T>::ElemType;
  AddOp()
      : BaseOp<T>(2, 2) {}
  ElemType operator()(ElemType x, ElemType y) const override {
    return x + y;
  }
};

template<typename T = float>
class SubtractOp : public BaseOp<T> {
public:
  using ElemType = typename BaseOp<T>::ElemType;
  SubtractOp()
      : BaseOp<T>(2, 2) {}
  ElemType operator()(ElemType x, ElemType y) const override {
    return x - y;
  }
};

template<typename T = float>
class MultiplyOp : public BaseOp<T> {
public:
  using ElemType = typename BaseOp<T>::ElemType;
  MultiplyOp()
      : BaseOp<T>(2, 3) {}
  ElemType operator()(ElemType x, ElemType y) const override {
    return x * y;
  }
};

template<typename T = float>
class DivideOp : public BaseOp<T> {
public:
  using ElemType = typename BaseOp<T>::ElemType;
  DivideOp()
      : BaseOp<T>(2, 3) {}
  ElemType operator()(ElemType x, ElemType y) const override {
    return x / y;
  }
};

REGISTER_OP('+', AddOp, float)
REGISTER_OP('-', SubtractOp, float)
REGISTER_OP('*', MultiplyOp, float)
REGISTER_OP('/', DivideOp, float)

int main() {
  std::string str_in;
  std::getline(std::cin, str_in);
  std::stack<OpMap::OpPointer> stack_ops;
  std::queue<OpMap::OpPointer> queue_subfix;
  OpMap::OpMapType* op_map = OpMap::get_op_map();

  if(str_in[0] == '-') {
    str_in = "0" + str_in;
  }

  // infix expression to suffix expression
  int start_idx = 0;
  for(int i = 0; i < str_in.size(); ++i) {
    char c = str_in[i];
    if(c == '+' || c == '-' || c == '*' || c == '/') {
      float n = std::stof(str_in.substr(start_idx, i - start_idx));
      start_idx = i + 1;

      typename OpMap::OpPointer op_number{new BaseOp<float>(n)};
      queue_subfix.push(op_number);

      OpMap::OpPointer op = (*op_map)[c];
      while(!stack_ops.empty()) {
        OpMap::OpPointer op_prv = stack_ops.top();
        if(op_prv->m_priority >= op->m_priority) {
          stack_ops.pop();
          queue_subfix.push(op_prv);
        } else {
          break;
        }
      }
      stack_ops.push(op);
    }
  }
  {
    float n = std::stof(str_in.substr(start_idx, str_in.size() - start_idx));
    OpMap::OpPointer op_number{new BaseOp<float>(n)};
    queue_subfix.push(op_number);
  }
  while(!stack_ops.empty()) {
    OpMap::OpPointer op = stack_ops.top();
    stack_ops.pop();
    queue_subfix.push(op);
  }

  // evaluation
  std::stack<OpMap::OpPointer> stack_eval;
  while(!queue_subfix.empty()) {
    OpMap::OpPointer op = queue_subfix.front();
    queue_subfix.pop();

    if(op->m_b_number) {
      stack_eval.push(op);
    } else {
      OpMap::OpPointer y = stack_eval.top();
      stack_eval.pop();
      OpMap::OpPointer x = stack_eval.top();
      stack_eval.pop();

      float val = (*op)(x->m_value, y->m_value);
      OpMap::OpPointer op_val{new BaseOp<float>(val)};
      stack_eval.push(op_val);
    }
  }

  std::cout << stack_eval.top()->m_value << "\n";

  return 0;
}
