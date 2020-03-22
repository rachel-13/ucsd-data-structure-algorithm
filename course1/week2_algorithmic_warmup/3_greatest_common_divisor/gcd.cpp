#include <iostream>
#include <cassert>

long long gcd_naive(long long a, long long b) {
  long long current_gcd = 1;
  for (int d = 2; d <= a && d <= b; d++) {
    if (a % d == 0 && b % d == 0) {
      if (d > current_gcd) {
        current_gcd = d;
      }
    }
  }
  return current_gcd;
}

long long gcd_fast(long long a, long long b) {
  long long current_gcd = 1;
  
  long long previousDivisor = b;
  long long remainder = a % b;
  
  if (remainder == 0) {
    return previousDivisor;
  } else {
    current_gcd = remainder;
  }
  
  return gcd_fast(previousDivisor, current_gcd);
}

int main() {
  int a, b;
  std::cin >> a >> b;
  std::cout << gcd_fast(a, b) << std::endl;
  return 0;
}
