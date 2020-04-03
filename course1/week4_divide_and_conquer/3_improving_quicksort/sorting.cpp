#include <iostream>
#include <vector>
#include <cstdlib>
#include <tuple>

using std::swap;
using std::vector;

/*
starter solution
*/
int partition2(vector<int> &a, int l, int r)
{
  int x = a[l];
  int j = l;
  for (int i = l + 1; i <= r; i++)
  {
    if (a[i] <= x)
    {
      j++;
      swap(a[i], a[j]);
    }
  }
  swap(a[l], a[j]);
  return j;
}

void randomized_quick_sort_2part(vector<int> &a, int l, int r)
{
  if (l >= r)
  {
    return;
  }

  int k = l + rand() % (r - l + 1);
  swap(a[l], a[k]);
  int m = partition2(a, l, r);

  randomized_quick_sort_2part(a, l, m - 1);
  randomized_quick_sort_2part(a, m + 1, r);
}

/*
own solution
*/

std::tuple<int, int> partition3(vector<int> &a, int l, int r)
{
  std::tuple<int, int> result;

  int x = a[l];

  int m1 = l;
  int m2 = r;
  int i = l;

  while (i <= m2 && i <= r)
  {
    if(a[i] < x) {
      swap(a[i], a[m1]);
      m1++;
      i++;
    } else if(a[i] > x) {
      swap(a[i], a[m2]);
      m2--;
    } else {
      i++;
    }
  }

  result = std::make_tuple(m1, m2);
  return result;
}

void randomized_quick_sort_3part(vector<int> &a, int l, int r)
{
  if (l >= r)
  {
    return;
  }

  int k = l + rand() % (r - l + 1);
  swap(a[l], a[k]);
  std::tuple<int, int> m = partition3(a, l, r);

  int m1 = std::get<0>(m);
  int m2 = std::get<1>(m);

  randomized_quick_sort_3part(a, l, m1 - 1);
  randomized_quick_sort_3part(a, m2 + 1, r);
}

void test_solution()
{
  while(true) {
    vector<int> testArr1;
    vector<int> testArr2;
    int inputSize = rand() % 100;
    std::cout << inputSize << "\n";

    for(int i = 0; i < inputSize; i++) {
      int randomInt = rand() % 100;
      testArr1.push_back(randomInt);
      testArr2.push_back(randomInt);
    }

    for(int i = 0; i < inputSize; i++) {
      std::cout << testArr1[i] << " ";
    }

    randomized_quick_sort_2part(testArr1, 0, testArr1.size() - 1);
    randomized_quick_sort_3part(testArr2, 0, testArr2.size() - 1);

    if(testArr1 == testArr2) {

      std::cout << "\n\nOK!\n";
      for(int i = 0; i < inputSize; i++) {
        std::cout << testArr1[i] << " ";
      }
      std::cout << "\nvs\n";
      for(int i = 0; i < inputSize; i++) {
        std::cout << testArr2[i] << " ";
      }

      std::cout << "\n==========================\n";
    } else {
      std::cout << "\n\nWrong result \n";
      for(int i = 0; i < inputSize; i++) {
        std::cout << testArr1[i] << " ";
      }
      std::cout << "\nvs\n";
      for(int i = 0; i < inputSize; i++) {
        std::cout << testArr2[i] << " ";
      }
      std::cout << "\n==========================\n";
      break;
    }
  }

  // vector<int> testArr = {9, 3, 8, 0, 2, 4, 8};
  // randomized_quick_sort_3part(testArr, 0, testArr.size() - 1);
}

int main()
{
  // test_solution();
  int n;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < a.size(); ++i) {
    std::cin >> a[i];
  }
  randomized_quick_sort_3part(a, 0, a.size() - 1);
  for (size_t i = 0; i < a.size(); ++i) {
    std::cout << a[i] << ' ';
  }
}
