#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;

int get_majority_element(vector<int> &a, int left, int right) {
  if (left == right) return -1;
  if (left + 1 == right) return a[left];
  
  sort(a.begin(), a.end());
  int pointer = 0;
  int count = 1;
  int halfCount = a.size() / 2;

  while (pointer < a.size())
  {
    if (pointer == a.size() - 1) {
      break;
    }

    if(a[pointer] == a[pointer + 1]) {
      count = count + 1;
      if (count > halfCount) {
        return 1;
      }
    } else {
      count = 1;
    }

    pointer = pointer + 1;
  }

  return -1;
}

int getMajorElement(vector<int> &a, int left, int right) {
  if (left == right) return -1;
  if (left + 1 == right) return a[left];
  sort(a.begin(), a.end());
  int count = 1;
  int halfCount = a.size() / 2;

  for(int i = left; i < right - 1; ++i) {

    if (i == right - 1) {
      if (count > halfCount) {
        return 1;
      }
    }

    if(a[i] == a[i+1]) {
      count = count + 1;
      if (count > halfCount) {
        return 1;
      }
    } else {
      count = 1;
    }
  }

  return -1;
}

void test_solution() {
  while(true) {
    vector<int> testArr;
    int inputSize = rand() % 100;
    std::cout << inputSize << "\n";

    for(int i = 0; i < inputSize; i++) {
      int randomInt = rand() % 5;
      testArr.push_back(randomInt);
    }
    sort(testArr.begin(), testArr.end());
    for(int i = 0; i < inputSize; i++) {
      std::cout << testArr[i] << " ";
    }

    int res1 = (get_majority_element(testArr, 0, testArr.size()) != -1);
    int res2 = (getMajorElement(testArr, 0, testArr.size()) != -1);

    if(res1 == res2) {
      std::cout << "\nOK!\n" << res1 << " vs " << res2 << "\n";
      std::cout << "\n==========================\n";
    } else {
      std::cout << "\nWrong result " << res1 << " vs " << res2 << "\n";
      std::cout << "\n==========================\n";
      break;
    }
  }

  // vector<int> testArr = { 4, 5, 8, 8 };
  // int res1 = (getMajorElement(testArr, 0, testArr.size()) != -1);
}

int main() {
  test_solution();
  // int n;
  // std::cin >> n;
  // vector<int> a(n);
  // for (size_t i = 0; i < a.size(); ++i) {
  //   std::cin >> a[i];
  // }
  // std::cout << (get_majority_element(a, 0, a.size()) != -1) << '\n';
}
