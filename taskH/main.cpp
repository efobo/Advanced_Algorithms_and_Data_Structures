#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const int LOG = 17;  // log2(MAXN) + 1

vector<vector<int>> tree;
vector<int> depth;
vector<vector<int>> binary_up;

void preparation_lca(const int n) {
  queue<int> q;
  depth[0] = 0;  // Корень дерева
  q.push(0);

  while (!q.empty()) {
    int node = q.front();
    q.pop();
    for (int neighbor : tree[node]) {
      if (depth[neighbor] == -1) {
        depth[neighbor] = depth[node] + 1;
        binary_up[neighbor][0] = node;
        q.push(neighbor);
      }
    }
  }

  for (int j = 1; j < LOG; ++j) {
    for (int i = 0; i < n; ++i) {
      if (binary_up[i][j - 1] != -1) {
        binary_up[i][j] = binary_up[binary_up[i][j - 1]][j - 1];
      }
    }
  }
}

string find_ans_LCA(int u, int v, int t) {
  if (depth[u] < depth[v]) {
    swap(u, v);  // Пусть u глубже
  }

  int distance = 0;
  int diff = depth[u] - depth[v];

  for (int i = 0; i < LOG; ++i) {
    if (diff & (1 << i)) {
      u = binary_up[u][i];
      distance += (1 << i);
      if (distance > t) {
        return "No";
      }
    }
  }

  if (u == v) {
    return "Yes";
  }

  for (int i = LOG - 1; i >= 0; --i) {
    if (binary_up[u][i] != binary_up[v][i]) {
      u = binary_up[u][i];
      v = binary_up[v][i];
      distance += (1 << (i + 1));
      if (distance > t) {
        return "No";
      }
    }
  }

  distance += 2;
  if (distance > t) {
    return "No";
  }

  return "Yes";
}

int main() {
  ifstream fin("../input.txt");
  int n;  // кол-во вершин в дереве
  fin >> n;

  tree.resize(n);
  depth.resize(n, -1);
  binary_up.resize(n, vector<int>(LOG, -1));

  // for (int i = 0; i < n; ++i) {
  //   tree[i].resize(n, 0);
  //   for (int j = 0; j < LOG; ++j) {
  //     binary_up[i][j] = -1;
  //   }
  // }

  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    fin >> u >> v;
    tree[u].push_back(v);
    tree[v].push_back(u);
  }

  preparation_lca(n);

  int q;  // кол-во запросов
  fin >> q;

  for (int i = 0; i < q; ++i) {
    int u, v;  // номера вершин
    int t;     // величина доступного заряда для перемещения
    fin >> u >> v >> t;
    cout << find_ans_LCA(u, v, t) << '\n';
  }
  fin.close();
  return 0;
}