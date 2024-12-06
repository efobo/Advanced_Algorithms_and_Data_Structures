#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

vector<int> xorValues;
unordered_map<int, vector<pair<int, int>>> tree;

struct Road {
  int u, v, w;
};

class BinaryTrie {
private:
  struct Node {
    Node* child[2] = {nullptr, nullptr};
  };

  Node* rootNode;

public:
  void deleteTrie(Node* node) {
    if (!node)
      return;
    deleteTrie(node->child[0]);
    deleteTrie(node->child[1]);
    delete node;
  }

  BinaryTrie() {
    rootNode = new Node();
  }
  ~BinaryTrie() {
    deleteTrie(rootNode);
  }

  void addNum(int num) {
    Node* current = rootNode;
    for (int bitPosition = 31; bitPosition >= 0; --bitPosition) {
      int currentBit = (num >> bitPosition) & 1;
      if (!current->child[currentBit]) {
        current->child[currentBit] = new Node();
      }
      current = current->child[currentBit];
    }
  }

  int findMaxXor(int num) {
    Node* current = rootNode;
    int maxResult = 0;
    for (int bitPosition = 31; bitPosition >= 0; --bitPosition) {
      int currentBit = (num >> bitPosition) & 1;
      int preferredBit;
      if (currentBit == 1) {
        preferredBit = 0;
      } else
        preferredBit = 1;

      if (current && current->child[preferredBit]) {
        maxResult |= (1 << bitPosition);
        current = current->child[preferredBit];
      } else if (current) {
        current = current->child[currentBit];
      }
    }
    return maxResult;
  }
};

void dfs(int node, int parent, int xorSum) {
  xorValues[node] = xorSum;
  for (const auto& neighbor : tree.at(node)) {
    int nextNode = neighbor.first;
    int weight = neighbor.second;
    if (nextNode != parent) {
      int newXorSum = xorSum ^ weight;
      dfs(nextNode, node, newXorSum);
    }
  }
}

int findMaxDiscount(int n, int f, const vector<Road>& roads) {
  for (const auto& road : roads) {
    tree[road.u].emplace_back(road.v, road.w);
    tree[road.v].emplace_back(road.u, road.w);
  }
  xorValues.resize(n + 1, -1);
  xorValues[f] = 0;

  dfs(f, -1, 0);

  BinaryTrie btrie;
  int maxDiscount = 0;

  for (int i = 1; i <= n; ++i) {
    if (i == f)
      continue;
    if (xorValues[i] != -1) {
      int maxXorForNode = btrie.findMaxXor(xorValues[i]);
      maxDiscount = max(maxDiscount, maxXorForNode);
      btrie.addNum(xorValues[i]);
    }
  }

  return maxDiscount;
}

int main() {
  // fstream fin;
  // fin.open("../input.txt");
  int n;  // кол-во мест
  int f;  // номер места с цветочным
  cin >> n >> f;

  vector<Road> roads;
  for (int i = 0; i < n - 1; ++i) {
    int u;
    int v;
    int w;
    cin >> u >> v >> w;
    roads.push_back({u, v, w});
  }
  // fin.close();
  int ans = findMaxDiscount(n, f, roads);

  cout << ans << "\n";

  return 0;
}
