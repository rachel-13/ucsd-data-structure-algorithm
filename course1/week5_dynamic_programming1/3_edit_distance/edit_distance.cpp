#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

int edit_distance(const string &str1, const int &str1Length, const string &str2, const int &str2Length) {
  if (str1 == str2) return 0;

  if(str1Length == 0) return str2Length;

  if(str2Length == 0) return str1Length;

  const int rows = str1Length + 1;
  const int columns = str2Length + 1;
  
  int table[rows][columns];

  for(int i = 0; i < rows; i ++) {
    table[i][0] = i;
  }

  for(int j = 0; j < columns; j ++) {
    table[0][j] = j;
  }

  for(int c = 1; c < columns; c++ ) {
    for(int r = 1; r < rows; r++ ) {
      int insertion = table[r][c -1] + 1;
      int deletion = table[r - 1][c] + 1;
      int match = table[r - 1][c - 1];
      int mismatch = table[r - 1][c - 1] + 1;

      if(str1[r - 1] == str2[c - 1]) {
        table[r][c] = std::min(insertion, std::min(deletion, match));
      } else {
        table[r][c] = std::min(insertion, std::min(deletion, mismatch));
      }
    }
  }
  int result = table[str1Length][str2Length];
  return result;
}

int main() {
  string str1;
  string str2;
  std::cin >> str1 >> str2;
  std::cout << edit_distance(str1, str1.size(), str2, str2.size()) << std::endl;
  return 0;
}
