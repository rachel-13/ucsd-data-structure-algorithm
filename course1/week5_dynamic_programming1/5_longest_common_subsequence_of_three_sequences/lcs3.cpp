#include <iostream>
#include <vector>
#include <cstring>

using std::vector;

int lcs3(vector<int> &aArr, const int &aSize, vector<int> &bArr, const int &bSize, vector<int> &cArr, const int &cSize) {
  const int row = aSize + 1;
  const int col = bSize + 1;
  const int depth = cSize + 1;
  int table[row][col][depth];
  memset(&table[0][0][0], 0, sizeof(table));

  for(int c = 1; c < col; c++) {
    for(int r = 1; r < row; r++) {
      for(int d = 1; d < depth; d++) {
        if(aArr[r - 1] == bArr[c - 1] && aArr[r - 1]== cArr[d - 1] ) {
          table[r][c][d] = table[r-1][c-1][d-1] + 1;
        } else {
          table[r][c][d] = std::max(table[r-1][c][d], std::max(table[r][c-1][d], table[r][c][d-1]));
        } 
      }
    }
  }

  int result = table[aSize][bSize][cSize];
  return result;
}

int main() {
  size_t an;
  std::cin >> an;
  vector<int> a(an);
  for (size_t i = 0; i < an; i++) {
    std::cin >> a[i];
  }
  size_t bn;
  std::cin >> bn;
  vector<int> b(bn);
  for (size_t i = 0; i < bn; i++) {
    std::cin >> b[i];
  }
  size_t cn;
  std::cin >> cn;
  vector<int> c(cn);
  for (size_t i = 0; i < cn; i++) {
    std::cin >> c[i];
  }
  std::cout << lcs3(a, a.size(), b, b.size(), c, c.size()) << std::endl;
}
