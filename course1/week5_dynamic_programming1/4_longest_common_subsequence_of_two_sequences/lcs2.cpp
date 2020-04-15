#include <iostream>
#include <vector>

using std::vector;

int lcs2(vector<int> &a, const int &aSize, vector<int> &b, const int &bSize) {

  const int row = aSize + 1;
  const int col = bSize + 1;
  int table[row][col];

  for(int i = 0; i <= row; i++) {
    table[i][0] = 0;
  }

  for(int j = 0; j <= col; j++) {
    table[0][j] = 0;
  }

  for(int c = 1; c < col; c++) {
    for(int r = 1; r < row; r++) {
      int insertion = table[r][c-1];
      int deletion = table[r-1][c];
      int match = table[r-1][c-1] + 1;
      int mismatch = table[r-1][c-1];

      if(a[r - 1] == b[c - 1]) {
        table[r][c] = std::max(insertion, std::max(deletion, match));
      } else {
        table[r][c] = std::max(insertion, std::max(deletion, mismatch));
      } 
    }
  }

  int result = table[aSize][bSize];
  return result;
}

int main() {
  size_t n;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < n; i++) {
    std::cin >> a[i];
  }

  size_t m;
  std::cin >> m;
  vector<int> b(m);
  for (size_t i = 0; i < m; i++) {
    std::cin >> b[i];
  }

  std::cout << lcs2(a, a.size(), b, b.size()) << std::endl;
}
