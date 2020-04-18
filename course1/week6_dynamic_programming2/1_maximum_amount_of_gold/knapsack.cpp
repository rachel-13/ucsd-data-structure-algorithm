#include <iostream>
#include <vector>

using std::vector;

int optimal_weight(const int &W, const vector<int> &w, const int &wSize) {
  
  const int row = wSize + 1;
  const int col = W + 1;
  vector < vector<int> > table(row, vector<int>(col));

  for(int i = 0; i < row; i++) {
    table[i][0] = 0;
  }

  for(int j = 0; j < col; j++) {
    table[0][j] = 0;
  }

  for(int r = 1; r < row; r++) {
    for(int c = 1; c < col; c++) {
      table[r][c] = table[r - 1][c];

      int currentItemWeight = w[r - 1];
      if (currentItemWeight <= c) {
        int newWeight = table[r - 1][c - currentItemWeight] + currentItemWeight;
        if (table[r][c] < newWeight) {
          table[r][c] = newWeight;
        }
      }
      
    }
  }

  int result = table[wSize][W];
  return result;
}

int main() {
  int n, W;
  std::cin >> W >> n;
  vector<int> w(n);
  for (int i = 0; i < n; i++) {
    std::cin >> w[i];
  }
  std::cout << optimal_weight(W, w, w.size()) << '\n';
}
