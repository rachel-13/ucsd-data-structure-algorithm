#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>

using std::vector;

int binary_search(const vector<int> &a, int lowerIndex, int upperIndex, int x) {
  if (upperIndex < lowerIndex) {
    return - 1;
  }

  int midIndex = lowerIndex + ((upperIndex - lowerIndex) / 2);
  int midValue = a[midIndex];
  
  if (x < midValue) {
    return binary_search(a, lowerIndex, midIndex - 1, x);
  } else if(x > midValue) {
    return binary_search(a, midIndex + 1, upperIndex, x);
  } else if(lowerIndex != midIndex) {
    return binary_search(a, lowerIndex, midIndex, x);
  } else {
    return midIndex;
  }
}

int linear_search(const vector<int> &a, int x) {
  for (size_t i = 0; i < a.size(); ++i) {
    if (a[i] == x) return i;
  }
  return -1;
}

void test_solution() {
  while(true) {
    int inputSize = rand() % 123456 + 10;
    int targetInt = rand() % 12345;
    vector<int> arr;
    for(int i = 0; i < inputSize; i++) {
      int randomNum= rand() % 123345;
      arr.push_back(randomNum);
    }
    sort(arr.begin(), arr.end());

    std::cout << inputSize << "\n";
    for(int j = 0; j < inputSize; j++) {
      std::cout << arr[j] << " ";
    }
    std::cout << "\n";
    std::cout << "Find " << targetInt << "\n";

    int res1 = binary_search(arr, 0, inputSize - 1, targetInt);
    int res2 = linear_search(arr, targetInt);

    if(res1 == res2) {
      std::cout << "\nOK!\n" << res1 << " vs " << res2 << "\n";
      std::cout << "\n==========================\n";
    } else {
      std::cout << "\nWrong result " << res1 << " vs " << res2 << "\n";
      std::cout << "\n==========================\n";
      break;
    }
  }

  // vector<int> testArr =  {0, 0, 0, 2, 3, 3, 4, 4, 5, 5, 5, 6, 6, 6, 6, 7, 8};
  // int x = 3;
  // int res = binary_search(testArr, 0, testArr.size() - 1, x);
}

int main() {
  // test_solution();
  int n;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < a.size(); i++) {
    std::cin >> a[i];
  }
  int m;
  std::cin >> m;
  vector<int> b(m);
  for (int i = 0; i < m; ++i) {
    std::cin >> b[i];
  }

  sort(a.begin(), a.end());
  for (int i = 0; i < m; ++i) {
    //replace with the call to binary_search when implemented
    std::cout << binary_search(a, 0, a.size() - 1, b[i]) << ' ';
  }
}
