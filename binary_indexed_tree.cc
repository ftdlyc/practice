#include <vector>

inline int lowbit(int x) {
  return x & (~x + 1); // return x & -x;
}

// 单点修改, 区间查询
template<typename T>
class BinaryIndexedTree1 {
public:
  using ElemType = T;
  using ContainerType = std::vector<ElemType>;

  struct BinaryIndexedTree1Node {
    BinaryIndexedTree1Node(BinaryIndexedTree1& bitree, int pos)
        : m_bitree(bitree)
        , m_pos(pos) {}

    const BinaryIndexedTree1Node operator+=(ElemType value) {
      m_bitree.add(m_pos, value);
      return *this;
    }

    const BinaryIndexedTree1Node operator-=(ElemType value) {
      m_bitree.add(m_pos, -value);
      return *this;
    }

    operator ElemType() {
      return m_bitree.query(m_pos);
    }

  private:
    BinaryIndexedTree1& m_bitree;
    int m_pos;
  };

  BinaryIndexedTree1()
      : m_array(1, 0) {}

  BinaryIndexedTree1(size_t sz)
      : m_array(sz <= 0 ? 1 : sz + 1, 0) {}

  size_t size() {
    return m_array.size() - 1;
  }

  void resize(size_t sz) {
    sz += 1;
    size_t old_sz = m_array.size();
    m_array.resize(sz);
    if(sz > old_sz) {
      auto it_begin = std::advance(m_array.begin(), old_sz);
      auto it_end = std::advance(m_array.begin(), sz);
      std::fill(it_begin, it_end, 0);
    }
  }

  void add(int pos, ElemType val) {
    if(pos <= 0) return;
    size_t sz = m_array.size();
    while(pos < sz) {
      m_array[pos] += val;
      pos += lowbit(pos);
    }
  }

  ElemType query(int pos) {
    ElemType ret = 0;
    while(pos > 0) {
      ret += m_array[pos];
      pos -= lowbit(pos);
    }
    return ret;
  }

  ElemType query(int left, int right) {
    return query(right) - query(left - 1);
  }

  BinaryIndexedTree1Node operator[](int pos) {
    return BinaryIndexedTree1Node(*this, pos);
  }

private:
  ContainerType m_array;
};

// 区间修改, 单点查询
// 通过差分(记录当前元素与前一个元素的差值)
template<typename T>
class BinaryIndexedTree2 {
public:
  using ElemType = T;
  using ContainerType = std::vector<ElemType>;

  BinaryIndexedTree2()
      : m_array(1, 0) {}

  BinaryIndexedTree2(size_t sz)
      : m_array(sz <= 0 ? 1 : sz + 1, 0) {}

  size_t size() {
    return m_array.size() - 1;
  }

  void resize(size_t sz) {
    sz += 1;
    size_t old_sz = m_array.size();
    m_array.resize(sz);
    if(sz > old_sz) {
      auto it_begin = std::advance(m_array.begin(), old_sz);
      auto it_end = std::advance(m_array.begin(), sz);
      std::fill(it_begin, it_end, 0);
    }
  }

  void add(int left, int right, ElemType val) {
    add(left, val);
    add(right + 1, -val);
  }

  ElemType query(int pos) {
    ElemType ret = 0;
    while(pos > 0) {
      ret += m_array[pos];
      pos -= lowbit(pos);
    }
    return ret;
  }

  const ElemType operator[](int pos) {
    return query(pos);
  }

private:
  void add(int pos, ElemType val) {
    if(pos <= 0) return;
    size_t sz = m_array.size();
    while(pos < sz) {
      m_array[pos] += val;
      pos += lowbit(pos);
    }
  }

  ContainerType m_array;
};

// 区间修改, 区间查询
template<typename T>
class BinaryIndexedTree3 {
public:
  using ElemType = T;
  using ContainerType = std::vector<ElemType>;

  BinaryIndexedTree3()
      : m_array1(1, 0)
      , m_array2(1, 0) {}

  BinaryIndexedTree3(size_t sz)
      : m_array1(sz <= 0 ? 1 : sz + 1, 0)
      , m_array2(sz <= 0 ? 1 : sz + 1, 0) {}

  size_t size() {
    return m_array1.size() - 1;
  }

  void resize(size_t sz) {
    sz += 1;
    size_t old_sz = m_array1.size();
    m_array1.resize(sz);
    m_array2.resize(sz);
    if(sz > old_sz) {
      {
        auto it_begin = std::advance(m_array1.begin(), old_sz);
        auto it_end = std::advance(m_array1.begin(), sz);
        std::fill(it_begin, it_end, 0);
      }
      {
        auto it_begin = std::advance(m_array2.begin(), old_sz);
        auto it_end = std::advance(m_array2.begin(), sz);
        std::fill(it_begin, it_end, 0);
      }
    }
  }

  void add(int left, int right, ElemType val) {
    add(m_array1, left, val);
    add(m_array1, right + 1, -val);
    add(m_array2, left, left * val);
    add(m_array2, right + 1, -(right + 1) * val);
  }

  ElemType query(int left, int right) {
    return query(right) - query(left - 1);
  }

private:
  void add(ContainerType& array, int pos, ElemType val) {
    if(pos <= 0) return;
    size_t sz = array.size();
    while(pos < sz) {
      array[pos] += val;
      pos += lowbit(pos);
    }
  }

  ElemType query(int pos) {
    return (pos + 1) * query(m_array1, pos) - query(m_array2, pos);
  }

  ElemType query(ContainerType& array, int pos) {
    ElemType ret = 0;
    while(pos > 0) {
      ret += array[pos];
      pos -= lowbit(pos);
    }
    return ret;
  }

  ContainerType m_array1;
  ContainerType m_array2;
};

int main() {
  BinaryIndexedTree3<int> tree(8);
  tree.add(2, 5, 3);
  tree.add(4, 7, -2);
  int ret1 = tree.query(1, 4);
  int ret2 = tree.query(3, 8);
  return 0;
}
