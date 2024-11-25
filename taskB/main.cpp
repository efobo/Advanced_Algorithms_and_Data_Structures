#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

vector<bool> bits;
int bits_size = 0;
int countOnes = 0;

void add(int S) {
  if (bits.empty()) {
    for (int i = 0; i <= S; i++) {
      if (i == S) {
        bits.push_back(true);
        countOnes++;
        bits_size++;
      } else {
        bits.push_back(false);
        bits_size++;
      }
    }
    return;
  }
  if (bits_size < S + 1) {
    for (int i = bits_size; i <= S; i++) {
      if (i == S) {
        bits.push_back(true);
        countOnes++;
        bits_size++;
      } else {
        bits.push_back(false);
        bits_size++;
      }
    }
    return;
  }
  if (!bits[S]) {
    bits[S] = true;
    countOnes++;
    return;
  }
  for (int i = S; i < bits_size; i++) {
    if (!bits[i]) {
      bits[i] = true;
      countOnes++;
      return;
    } else {
      bits[i] = false;
      countOnes--;
    }
    if (i + 1 == bits_size) {
      bits.push_back(true);
      countOnes++;
      bits_size++;
      return;
    }
  }
}

void sub(int S) {
  if (bits[S]) {
    bits[S] = false;
    countOnes--;
    return;
  }
  for (int i = S; i < bits_size; i++) {
    if (bits[i]) {
      bits[i] = false;
      countOnes--;
      return;
    } else {
      bits[i] = true;
      countOnes++;
    }
  }
}

int main() {
  ifstream fin("../input.txt");
  int n;
  fin >> n;

  for (int i = 0; i < n; i++) {
    char op;
    int S;
    fin >> op >> S;
    if (op == '+') {
      add(S);
    }
    if (op == '-') {
      sub(S);
    }
    cout << countOnes << '\n';
  }
}
