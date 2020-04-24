#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <limits>

using std::vector;
using std::string;
using std::max;
using std::min;

struct Pair {
  long long int min;
  long long int max;
};

long long int eval(long long int a, char op, long long int b) {
  if (op == '*') {
    return a * b;
  } else if (op == '+') {
    return a + b;
  } else if (op == '-') {
    return a - b;
  } else {
    assert(0);
  }
}

Pair minMax(int i, int j, vector<vector<long long int>> minTable, vector<vector<long long int>> maxTable, vector<char> ops) {
  Pair pair = Pair();
  long long int min = std::numeric_limits<long long int>::max();
  long long int max = std::numeric_limits<long long int>::min();

  for(int k = i; k <= j - 1; k++) {
    long long int a = eval(maxTable[i][k], ops[k], maxTable[k+1][j]);
    long long int b = eval(maxTable[i][k], ops[k], minTable[k+1][j]);
    long long int c = eval(minTable[i][k], ops[k], maxTable[k+1][j]);
    long long int d = eval(minTable[i][k], ops[k], minTable[k+1][j]);

    min = std::min(min, std::min(a, std::min(b, std::min(c, d))));
    max = std::max(max, std::max(a, std::max(b, std::max(c, d))));
  }

  pair.min = min;
  pair.max = max;
  return pair;
}

long long int get_maximum_value(const string &exp) {
  long long int result = 0;
  vector<int> digit;
  vector<char> ops;

  for(int i = 0; i < exp.size() + 1; i++) {
    if(isdigit(exp[i])) {
      int d = exp[i] - '0';
      digit.push_back(d);
    } else {
      ops.push_back(exp[i]);
    }
  }
  ops.pop_back(); // to remove null terminating char \0 at the end
  
  const int n = digit.size();

  vector< vector<long long int> > minTable(n, vector<long long int>(n));
  vector< vector<long long int> > maxTable(n, vector<long long int>(n));

  for(int i = 0; i < n; i++) {
    minTable[i][i] = digit[i];
    maxTable[i][i] = digit[i];
  }

  for(int s = 1; s < n; s++) {
    for(int i = 0; i < n - s; i++) {
      int j = i + s;
      Pair pair = minMax(i,j, minTable, maxTable, ops);
      minTable[i][j] = pair.min;
      maxTable[i][j] = pair.max;
    }
  }

  result = maxTable[0][n - 1];
  return result;
}

int main() {
  string s;
  std::cin >> s;
  // s = "5-8+7*4-8+9";
  std::cout << get_maximum_value(s) << '\n';
}
