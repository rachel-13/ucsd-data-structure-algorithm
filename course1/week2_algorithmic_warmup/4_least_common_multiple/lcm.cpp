#include <iostream>

long long lcm_naive(int a, int b) {
  for (long l = 1; l <= (long long) a * b; ++l)
    if (l % a == 0 && l % b == 0)
      return l;

  return (long long) a * b;
}

long long gcd(long long a, long long b) {
  long long current_gcd = 1;

  long long previousDivisor = b;
  long long remainder = a % b;

  if (remainder == 0) {
    return previousDivisor;
  }

  return gcd(previousDivisor, remainder);
}

long long lcm_fast(long long a, long long b) {
  return (a * b) / gcd(a,b);
}

int main() {
  int a, b;
  std::cin >> a >> b;
  std::cout << lcm_fast(a, b) << std::endl;
  return 0;
}
