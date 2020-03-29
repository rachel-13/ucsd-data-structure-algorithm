#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;

vector<int> redistributeSummands(vector<int> summands, int temp_n) {
  vector<int> newSummands;

  for(int i = summands.size() - 1; i >= 0; i--) {
    if(temp_n > 0) {
      int newSummand = summands[i] + 1;
      temp_n = temp_n - 1;
      newSummands.push_back(newSummand);
    } else {
      newSummands.push_back(summands[i]);
    }
  }
  sort(newSummands.begin(), newSummands.end());
  return newSummands;
}

vector<int> optimal_summands(int n) {
  vector<int> summands;
  int summand = 1;
  int temp_n = n;

  while(true) {
    if(n == 2) {
      summands.push_back(n);
      break;
    }

    if(summand > temp_n) {
      vector<int> alteredSummands = redistributeSummands(summands, temp_n);
      // summand = temp_n;
      return alteredSummands;
    }

    temp_n = temp_n - summand;
    summands.push_back(summand);

    if (temp_n == 0) {
      break;
    }

    summand = summand + 1;

  }
  return summands;
}



int main() {
  int n;
  std::cin >> n;
  vector<int> summands = optimal_summands(n);
  std::cout << summands.size() << '\n';
  for (size_t i = 0; i < summands.size(); ++i) {
    std::cout << summands[i] << ' ';
  }
}
