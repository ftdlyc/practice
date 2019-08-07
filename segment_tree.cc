#include <limits>
#include <math.h>
#include <vector>

template<typename T>
class SegmentTree {
public:
  struct Node;
  using ElemType = T;
  using ContainerType = std::vector<Node>;
  using SequenceType = std::vector<ElemType>;
  using SizeType = size_t;

  struct Node {
    int start;
    int end;
    ElemType data;
    ElemType mark;
    Node()
        : start(-1)
        , end(-1)
        , data(0)
        , mark(0) {}
    Node(int start_, int end_)
        : start(start_)
        , end(end_)
        , data(0)
        , mark(0) {}
    void update_mark(ElemType value) { mark += value; }
    void clear_mark() { mark = 0; }
  };

  SegmentTree(const SequenceType& seq) {
    if(seq.empty()) return;
    int sz = seq.size();
    int total_sz = std::pow(2, std::ceil(std::log2(sz))) * 2;
    m_tree.resize(total_sz);
    build(1, 0, sz - 1, seq);
  }

  ElemType query(int start, int end) {
    return end < start ? 0 : query_impl(1, start, end);
  }

  void update(int index, int value) {
    if(index < 0) return;
    update_impl(1, index, value);
  }

  void update(int start, int end, int value) {
    if(end < start) return;
    update_impl(1, start, end, value);
  }

private:
  void build(int root, int start, int end, const SequenceType& seq) {
    Node& node = m_tree[root];
    node.start = start;
    node.end = end;
    if(start == end) {
      node.data = seq[start];
    } else {
      int mid = (start + end) / 2;
      build(root * 2, start, mid, seq);
      build(root * 2 + 1, mid + 1, end, seq);
      node.data = std::min(m_tree[root * 2].data, m_tree[root * 2 + 1].data);
    }
  }

  ElemType query_impl(int root, int start, int end) {
    const Node& node = m_tree[root];
    if(node.start > end || node.end < start) return std::numeric_limits<ElemType>::max();
    if(node.start >= start && node.end <= end) return node.data;
    push_down(root);
    ElemType n1 = query_impl(root * 2, start, end);
    ElemType n2 = query_impl(root * 2 + 1, start, end);
    return n1 < n2 ? n1 : n2;
  }

  void update_impl(int root, int index, int value) {
    Node& node = m_tree[root];
    if(node.start == node.end) {
      if(node.start == index) node.data += value;
      return;
    }
    push_down(root);
    int mid = (node.start + node.end) / 2;
    if(index <= mid) {
      update_impl(root * 2, index, value);
    } else {
      update_impl(root * 2 + 1, index, value);
    }
    node.data = std::min(m_tree[root * 2].data, m_tree[root * 2 + 1].data);
  }

  void update_impl(int root, int start, int end, int value) {
    Node& node = m_tree[root];
    if(node.start > end || node.end < start) return;
    if(node.start >= start && node.end <= end) {
      node.update_mark(value);
      node.data += value;
      return;
    }
    update_impl(root * 2, start, end, value);
    update_impl(root * 2 + 1, start, end, value);
    node.data = std::min(m_tree[root * 2].data, m_tree[root * 2 + 1].data);
  }

  void push_down(int root) {
    Node& node = m_tree[root];
    if(node.mark != 0 && node.start != node.end) {
      m_tree[root * 2].update_mark(node.mark);
      m_tree[root * 2 + 1].update_mark(node.mark);
      m_tree[root * 2].data += node.mark;
      m_tree[root * 2 + 1].data += node.mark;
    }
    node.clear_mark();
  }

  ContainerType m_tree;
};

int main() {
  std::vector<int> v = {5, 9, 7, 4, 6, 1};
  SegmentTree<int> seg_tree(v);
  int n1 = seg_tree.query(2, 4);
  seg_tree.update(3, 4);
  int n2 = seg_tree.query(2, 4);
  seg_tree.update(4, 5, 2);
  int n3 = seg_tree.query(2, 4);
  return 0;
}
