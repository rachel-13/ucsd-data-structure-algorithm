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
  int min;
  int max;
};

long long eval(long long a, char op, long long b) {
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

Pair minMax(int i, int j, vector<vector<int>> minTable, vector<vector<int>> maxTable, vector<char> ops) {
  Pair pair = Pair();
  int min = std::numeric_limits<int>::max();
  int max = std::numeric_limits<int>::min();

  for(int k = i; k <= j - 1; k++) {
    int a = eval(maxTable[i][k], ops[k], maxTable[k+1][j]);
    int b = eval(maxTable[i][k], ops[k], minTable[k+1][j]);
    int c = eval(minTable[i][k], ops[k], maxTable[k+1][j]);
    int d = eval(minTable[i][k], ops[k], minTable[k+1][j]);

    min = std::min(min, std::min(a, std::min(b, std::min(c, d))));
    max = std::max(max, std::max(a, std::max(b, std::max(c, d))));
  }

  pair.min = min;
  pair.max = max;
  return pair;
}

long long get_maximum_value(const string &exp) {
  int result = 0;
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

  vector< vector<int> > minTable(n, vector<int>(n));
  vector< vector<int> > maxTable(n, vector<int>(n));

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
