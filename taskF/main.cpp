#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
  ifstream fin;
  fin.open("../input.txt");
  int n, m;
  fin >> n >> m;
  string str;
  fin >> str;
  int str_pointer = 0;
  int prev_str_pointer = 0;
  int word_pointer = 0;
  bool fool_word = false;
  string ans = "NO";

  vector<string> words(m);
  for (int i = 0; i < m; ++i) {
    fin >> words[i];
    for (int j = str_pointer; j < n; ++j) {
      if (words[i][word_pointer] == str[j]) {
        word_pointer++;
        if (word_pointer == (int)words[i].size()) {
          word_pointer = 0;
          str_pointer = j + 2;
          fool_word = true;
          break;
        }
      } else if (words[i][0] == str[j]) {
        word_pointer = 1;
        if (words[i].size() == 1) {
          word_pointer = 0;
          str_pointer = j + 3;
          fool_word = true;
          break;
        }
      } else if (word_pointer != 0) {
        word_pointer = 0;
      }
    }
    if (word_pointer == 0 && i == m - 1 && fool_word) {
      ans = "YES";
    }
    if (str_pointer == prev_str_pointer) {
      break;
    } else {
      prev_str_pointer = str_pointer;
    }
    fool_word = false;
  }
  cout << ans << '\n';
}
