#include <iostream>
#include <vector>

using std::vector;

int partition_dp(vector<int> &A, const int&n, const int &thresholdFor3EqualParts) {
  int result = 0;
  
  const int row = n + 1;
  const int col = thresholdFor3EqualParts + 1;
  int table[row][col];

  for(int i = 0; i < row; i++) {
    table[i][0] = 0;
  }

  for(int j = 0; j < col; j++) {
    table[0][j] = 0;
  }

  for(int r = 1; r < row; r++) {
    for(int c = 1; c < col; c++) {
      table[r][c] = table[r - 1][c];

      int value = A[r - 1];
      if (value <= c) {
        int newWeight = table[r - 1][c - value] + value;
        if (table[r][c] < newWeight) {
          table[r][c] = newWeight;
        }
      }
    }
  }

  if(table[n][thresholdFor3EqualParts] == thresholdFor3EqualParts) {
    result = 1;
  }

  return result;
}

int partition3(vector<int> &A, const int &n) {
  int result = 0;
  if (n == 1) {
    return result;
  }

  int sumOfAllNumbers = 0;
  for(int i = 0; i < n; i++) {
    sumOfAllNumbers = sumOfAllNumbers + A[i];
  }

  if (sumOfAllNumbers % 3 > 0 || sumOfAllNumbers < 3) {
    return result;
  }

  int thresholdFor3EqualParts = sumOfAllNumbers / 3;

  result = partition_dp(A, A.size(), thresholdFor3EqualParts);

  return result;
}

int main() {
  int n;
  std::cin >> n;
  vector<int> A(n);
  for (size_t i = 0; i < A.size(); ++i) {
    std::cin >> A[i];
  }
  std::cout << partition3(A, n) << '\n';
}
