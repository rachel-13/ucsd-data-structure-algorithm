#include <iostream>
#include <vector>

using std::vector;

void removeFromVector(vector<int> &A, int target) {
  for(int i = 0; i < A.size(); i++) {
    if(A[i] == target) {
      A.erase(A.begin() + i);
      return;
    }
  }
}

bool isElementUnusued(vector<int> &A, int target) {
  for(int i = 0; i < A.size(); i++) {
    if(A[i] == target) {
      return true;
    }
  }
  return false;
}

int partition_dp(vector<int> &A, const int&n, const int &thresholdFor3EqualParts) {
  int result = 0;
  
  const int row = n + 1;
  const int col = thresholdFor3EqualParts + 1;
  int table[7][5];

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

  vector<int> source = A;

  vector<int> part1 = {};
  vector<int> part2 = {};
  vector<int> part3 = {};

  int element1 = A[n-1];
  int element2 = A[n-2];
  int element3 = A[n-3];

  int part1_limit_pointer = thresholdFor3EqualParts;
  int part1_value_pointer = n;

  int part2_limit_pointer = thresholdFor3EqualParts;
  int part2_value_pointer = n - 1;

  int part3_limit_pointer = thresholdFor3EqualParts;
  int part3_value_pointer = n - 2;


  if(table[n][thresholdFor3EqualParts] == thresholdFor3EqualParts) {
  
    while(source.size() > 0) {
      if(table[part1_value_pointer - 1][part1_limit_pointer - element1] + element1 >= table[part1_value_pointer - 1][part1_limit_pointer] &&
      isElementUnusued(source, element1) &&
      table[part1_value_pointer - 1][part1_limit_pointer - element1] + element1 <= part1_limit_pointer) 
      {
          part1.push_back(element1);
          removeFromVector(source, element1);
          part1_limit_pointer = part1_limit_pointer - element1;
          part1_value_pointer = part1_value_pointer - 1;
          element1 = A[part1_value_pointer - 1]; 
      } else {
          part1_value_pointer = part1_value_pointer - 1;
          element1 = A[part1_value_pointer - 1];
      }

      if(table[part2_value_pointer - 1][part2_limit_pointer - element2] + element2 >= table[part2_value_pointer - 1][part2_limit_pointer] &&
        isElementUnusued(source, element2) &&
        table[part2_value_pointer - 1][part2_limit_pointer - element2] + element2 <= part2_limit_pointer) 
      {
          part2.push_back(element2);
          removeFromVector(source, element2);
          part2_limit_pointer = part2_limit_pointer - element2;
          part2_value_pointer = part2_value_pointer - 1;
          element2 = A[part2_value_pointer - 1];
      } else {
        part2_value_pointer = part2_value_pointer - 1;
        element2 = A[part2_value_pointer - 1];
      }

      if(table[part3_value_pointer - 1][part3_limit_pointer - element3] + element3 >= table[part3_value_pointer - 1][part3_limit_pointer] && 
        isElementUnusued(source, element3) &&
        table[part3_value_pointer - 1][part3_limit_pointer - element3] + element3 <= part3_limit_pointer) 
      {
          part3.push_back(element3);
          removeFromVector(source, element3);
          part3_limit_pointer = part3_limit_pointer - element3;
          part3_value_pointer = part3_value_pointer - 1;
          element3 = A[part3_value_pointer - 1];
      } else {
        part3_value_pointer = part3_value_pointer - 1;
        element3 = A[part3_value_pointer - 1];
      } 
    }

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
