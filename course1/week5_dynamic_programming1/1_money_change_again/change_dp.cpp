#include <iostream>
#include <cmath>

int get_change(int m) {
  
  int money = m;
  int minNumCoins[money + 1];
  minNumCoins[0] = 0;

  int coins[3] = { 1, 3, 4 };

  for(int m = 1; m <= money; m++) {
    minNumCoins[m] = INFINITY;
    for(int i = 0; i <= 2; i++) {
      if (m >= coins[i]) {
        int currentNumCoin = minNumCoins[m - coins[i]] + 1;
        if(currentNumCoin < minNumCoins[m]) {
          minNumCoins[m] = currentNumCoin;
        }
      }
    }
  }
  
  return minNumCoins[money];
}

int main() {
  int m;
  std::cin >> m;
  std::cout << get_change(m) << '\n';
}
