#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;

vector<int> optimal_sequence(int n) {
  std::vector<int> sequence;
  std::vector<int> cache(n+1);
  cache[0] = 0;
  cache[1] = 0;

  // building a cache of how many operations it takes to get to 2,3,4...n
  for(int i = 2; i <= n; i++) {
    cache[i] = cache[i - 1] + 1;

    if(i % 3 == 0) {
      cache[i] = std::min(cache[i / 3] + 1, cache[i]);
    } else if(i % 2 == 0) {
      cache[i] = std::min(cache[i / 2] + 1, cache[i]);
    }
  }

  for (int i = n; i > 1; ) {
      sequence.push_back(i);
      
      if (i % 3 == 0 && (cache[i / 3] == cache[i] - 1))
        i = i / 3; 
      else if (i % 2 == 0 && (cache[i / 2] == cache[i] - 1))
        i = i / 2;
      else if (cache[i - 1] == cache[i] - 1)
        i = i - 1;
  }
  sequence.push_back(1);

  reverse(sequence.begin(), sequence.end());
  return sequence;
}

int main() {
  int n;
  std::cin >> n;
  vector<int> sequence = optimal_sequence(n);
  std::cout << sequence.size() - 1 << std::endl;
  for (size_t i = 0; i < sequence.size(); ++i) {
    std::cout << sequence[i] << " ";
  }
}
