#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int n;
vector<int> arr;
int block_size;
vector<int> blocks;

void BuildBlocks(int n) {
  for (int i = 0; i < n; i++) {
    if (arr[i] == 0) {
      int index = i / block_size;
      blocks[index]++;
    }
  }
}

void Update(const int pos, const int new_val) {
  int block_index = pos / block_size;
  if (arr[pos] == 0) {
    blocks[block_index]--;
  }
  if (new_val == 0) {
    blocks[block_index]++;
  }
  arr[pos] = new_val;
}

int FindKthZero(const int left_border, const int right_border, const int k) {
  if (k > n) {
    return -1;
  }
  int count_zeros = 0;

  int const first_block = left_border / block_size;
  int const last_block = right_border / block_size;

  for (int i = first_block; i <= last_block; i++) {
    int const first_block_index = i * block_size;
    int const last_block_index = min(((i + 1) * block_size) - 1, n - 1);

    if (first_block_index >= left_border && last_block_index <= right_border) {
      if (count_zeros + blocks[i] < k) {
        count_zeros += blocks[i];
      } else {
        for (int j = first_block_index; j <= last_block_index; j++) {
          if (arr[j] == 0) {
            count_zeros += 1;
            if (count_zeros >= k) {
              return j;
            }
          }
        }
      }
    } else {
      int start_check_index = max(first_block_index, left_border);
      int end_check_index = min(last_block_index, right_border);
      for (int j = start_check_index; j <= end_check_index; j++) {
        if (arr[j] == 0) {
          count_zeros += 1;
          if (count_zeros >= k) {
            return j;
          }
        }
      }
    }
  }
  return -1;
}

int main() {
  ifstream fin("../input.txt");
  fin >> n;
  arr.resize(n);
  for (int i = 0; i < n; i++) {
    fin >> arr[i];
  }

  block_size = sqrt(n);
  blocks.resize((n + block_size - 1) / block_size, 0);
  BuildBlocks(n);

  int m;
  fin >> m;
  string answer = "";
  for (int i = 0; i < m; i++) {
    char type;
    fin >> type;
    if (type == 'u') {
      int index;
      int new_val;
      fin >> index >> new_val;
      Update(index - 1, new_val);
    } else if (type == 's') {
      int left_border = -1;
      int right_border = -1;
      int k = -1;
      fin >> left_border >> right_border >> k;
      int result = FindKthZero(left_border - 1, right_border - 1, k);
      if (result == -1) {
        answer.append("-1 ");
      } else {
        answer.append(to_string(result + 1));
        answer.append(" ");
      }
    }
  }
  fin.close();
  cout << answer << '\n';
  return 0;
}
