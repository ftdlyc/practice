#include <vector>

class UnionFind {
public:
  UnionFind(int sz)
      : m_id(sz)
      , m_sz(sz, 1)
      , m_count(sz) {
    for(int i = 0; i < sz; ++i) {
      m_id[i] = i;
    }
  }

  int find(int p) {
    int root = p;
    while(m_id[root] != root) {
      root = m_id[root];
    }
    while(p != root) {
      int q = p;
      p = m_id[p];
      m_id[q] = root;
    }
    return root;
  }

  void join(int p, int q) {
    int root_p = find(p);
    int root_q = find(q);
    if(root_p == root_q) return;
    if(m_sz[root_p] >= m_sz[root_q]) {
      m_id[root_q] = root_p;
      m_sz[root_p] += m_sz[root_q];
    } else {
      m_id[root_p] = root_q;
      m_sz[root_q] += m_sz[root_p];
    }
    --m_count;
  }

  bool is_connect(int p, int q) {
    return find(p) == find(q);
  }

  int count() {
    return m_count;
  }

  void flatten(bool consecutive) {
    if(consecutive) {
      int k = 0;
      for(int i = 0; i < m_id.size(); ++i) {
        if(m_id[i] == i) {
          m_id[i] = k;
          ++k;
        } else {
          m_id[i] = m_id[m_id[i]];
        }
      }
    } else {
      for(int i = 0; i < m_id.size(); ++i) {
        m_id[i] = m_id[m_id[i]];
      }
    }
  }

private:
  std::vector<int> m_id;
  std::vector<size_t> m_sz;
  int m_count;
};

int main() {
  UnionFind uf(8);
  uf.join(1, 5);
  uf.join(1, 4);
  uf.join(4, 7);
  uf.join(0, 2);
  uf.join(2, 3);
  uf.join(3, 6);
  uf.join(6, 7);
  uf.flatten(true);
  return 0;
}
