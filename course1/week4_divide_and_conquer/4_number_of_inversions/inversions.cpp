#include <iostream>
#include <vector>

using std::vector;

int findInversionCount(vector<int> &arr, int n)
{
	int inversionCount = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
			if (arr[i] > arr[j])
				inversionCount++;
	}
	return inversionCount;
}

/*
own solution
*/

long long mergeArrays(vector<int> &a, vector<int> &b, size_t left,  size_t ave, size_t right) {
  long long inversions = 0;
  int i = left;
  int j = ave + 1;
  int k = left;

  while(i <= ave && j <= right) {
    int leftArrElement = a[i];
    int rightArrElement = a[j];

    if(leftArrElement <= rightArrElement) {
      b[k] = leftArrElement;
      i++;
      k++;
    } else {
      b[k] = rightArrElement;
      j++;
      k++;
      inversions += (ave - i + 1);
    }
  }

  while(i <= ave) {
    b[k] = a[i];
    k++;
    i++;
  }

  while(j <= right) {
    b[k] = a[j];
    k++;
    j++;
  }

  for(int h = left; h <= right; h++) {
    a[h] = b[h];
  }

  return inversions;
}


long long get_number_of_inversions(vector<int> &a, vector<int> &b, size_t left, size_t right) {
  long long number_of_inversions = 0;
  // if (right <= left + 1) return number_of_inversions;
  
  if(right > left) {
    size_t ave = left + (right - left) / 2;
    number_of_inversions += get_number_of_inversions(a, b, left, ave);
    number_of_inversions += get_number_of_inversions(a, b, ave + 1, right);
    number_of_inversions += mergeArrays(a, b, left, ave, right);
  }
  return number_of_inversions;
}

void test_solution() {
  while(true) {
    vector<int> testArr1;
    vector<int> testArr3;
    int inputSize = rand() % 10 + 10;
    std::cout << inputSize << "\n";

    for(int i = 0; i < inputSize; i++) {
      int randomInt = rand() % 10;
      testArr1.push_back(randomInt);
      testArr3.push_back(randomInt);
    }

    for(int i = 0; i < inputSize; i++) {
      std::cout << testArr1[i] << " ";
    }

    vector<int> testArr2(testArr1.size());
    vector<int> testArr4(testArr1.size());

    int res1 = get_number_of_inversions(testArr1, testArr2, 0, testArr1.size() - 1);
    int res2 = findInversionCount(testArr3, testArr1.size());

    if(res1 == res2) {

      std::cout << "\n\nOK!\n";
      
      std::cout << res1 << " ";
      
      std::cout << "\nvs\n";
      
      std::cout << res2 << " ";
      
      std::cout << "\n==========================\n";
    } else {
      std::cout << "\n\nWrong result \n";
      
      std::cout << res1 << " ";
      
      std::cout << "\nvs\n";
      
      std::cout << res2 << " ";
      
      std::cout << "\n==========================\n";
      break;
    }
  }

  // vector<int> testArr = {2,3,9,2,9};
  // vector<int> testArrr(testArr.size());
  // int res = get_number_of_inversions(testArr, testArrr, 0, testArr.size() - 1);
  // std::cout << res;
}

int main() {
  // test_solution();

  int n;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < a.size(); i++) {
    std::cin >> a[i];
  }
  vector<int> b(a.size());
  std::cout << get_number_of_inversions(a, b, 0, a.size() - 1) << '\n';
}
