#include <fstream>
#include <iostream>
#include <random>
#include <tuple>
using namespace std;

struct TreapNode {
  int key;
  int value;
  int priority;
  int size;
  int64_t sum;
  TreapNode* left;
  TreapNode* right;

  TreapNode(int k, int v)
      : key(k), value(v), priority(rand()), size(1), sum(v), left(nullptr), right(nullptr) {
  }
};

TreapNode* odd_tree;
TreapNode* even_tree;

int get_size(TreapNode* node) {
  return node ? node->size : 0;
}

int64_t get_sum(TreapNode* node) {
  return node ? node->sum : 0;
}

void update(TreapNode* node) {
  if (node) {
    node->size = get_size(node->left) + get_size(node->right) + 1;
    node->sum = get_sum(node->left) + get_sum(node->right) + node->value;
  }
}
void destroy_tree(TreapNode* node) {
  if (!node)
    return;
  destroy_tree(node->left);
  destroy_tree(node->right);
  delete node;
}

tuple<TreapNode*, TreapNode*> split(TreapNode* node, int key) {
  if (!node)
    return {nullptr, nullptr};
  if (key <= get_size(node->left)) {
    auto [left, right] = split(node->left, key);
    node->left = right;
    update(node);
    return {left, node};
  } else {
    auto [left, right] = split(node->right, key - get_size(node->left) - 1);
    node->right = left;
    update(node);
    return {node, right};
  }
}

TreapNode* merge(TreapNode* left, TreapNode* right) {
  if (!left || !right)
    return left ? left : right;
  if (left->priority > right->priority) {
    left->right = merge(left->right, right);
    update(left);
    return left;
  } else {
    right->left = merge(left, right->left);
    update(right);
    return right;
  }
}

TreapNode* insert_odd_tree(int key, int value) {
  TreapNode* new_node = new TreapNode(key, value);
  auto [left, right] = split(odd_tree, key);
  return merge(merge(left, new_node), right);
}
TreapNode* insert_even_tree(int key, int value) {
  TreapNode* new_node = new TreapNode(key, value);
  auto [left, right] = split(even_tree, key);
  return merge(merge(left, new_node), right);
}

int64_t range_sum_odd_tree(int l, int r) {
  auto [left, middle] = split(odd_tree, l);
  auto [middle_part, right] = split(middle, r - l + 1);
  int64_t result = get_sum(middle_part);
  odd_tree = merge(merge(left, middle_part), right);
  return result;
}

int64_t range_sum_even_tree(int l, int r) {
  auto [left, middle] = split(even_tree, l);
  auto [middle_part, right] = split(middle, r - l + 1);
  int64_t result = get_sum(middle_part);
  even_tree = merge(merge(left, middle_part), right);
  return result;
}

void swap_segments(int l, int r) {
  int odd_l = (l + 1) / 2;
  int odd_r = r / 2;
  int even_l = l / 2;
  int even_r = (r - 1) / 2;

  auto [odd_left, odd_middle] = split(odd_tree, odd_l);
  auto [odd_segment, odd_right] = split(odd_middle, odd_r - odd_l + 1);

  auto [even_left, even_middle] = split(even_tree, even_l);
  auto [even_segment, even_right] = split(even_middle, even_r - even_l + 1);

  odd_tree = merge(merge(odd_left, even_segment), odd_right);
  even_tree = merge(merge(even_left, odd_segment), even_right);
}

int main() {
  ifstream fin("input.txt");
  int suite_num = 0;

  while (true) {
    int n, r;
    if (!(fin >> n >> r))
      break;
    if (n == 0 && r == 0)
      break;
    odd_tree = nullptr;
    even_tree = nullptr;
    for (int i = 0; i < n; ++i) {
      int val;
      fin >> val;
      if (i % 2 == 0) {
        odd_tree = insert_odd_tree(i / 2, val);
      } else {
        even_tree = insert_even_tree(i / 2, val);
      }
    }

    cout << "Suite " << ++suite_num << ":\n";
    for (int i = 0; i < r; ++i) {
      int q_type, l, rr;
      fin >> q_type >> l >> rr;
      if (q_type == 1) {
        l--;
        rr--;
        swap_segments(l, rr);
      } else if (q_type == 2) {
        l--;
        rr--;
        if (l == rr && l % 2 == 0) {
          int64_t odd_sum = range_sum_odd_tree((l + 1) / 2, rr / 2);
          cout << odd_sum << "\n";
        } else if (l == rr && l % 2 == 1) {
          int64_t even_sum = range_sum_even_tree(l / 2, (rr - 1) / 2);
          cout << even_sum << "\n";
        } else {
          int64_t odd_sum = range_sum_odd_tree((l + 1) / 2, rr / 2);
          int64_t even_sum = range_sum_even_tree(l / 2, (rr - 1) / 2);
          int64_t sec_req_result = odd_sum + even_sum;
          cout << sec_req_result << '\n';
        }
      }
    }
    cout << "\n";
    destroy_tree(odd_tree);
    destroy_tree(even_tree);
    odd_tree = nullptr;
    even_tree = nullptr;
  }
  return 0;
}
