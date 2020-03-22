#include <iostream>

int get_change(int m) {
  
  int one_denom = 1;
  int five_denom = 5;
  int ten_denom = 10;

  int fives_unit = 0;
  int tens_unit = 0;
  int ones_unit = 0;

  while(m > 0) {
    if(m / ten_denom > 0) {
      tens_unit = m / ten_denom;
      m = m - (tens_unit * ten_denom);
    }

    if(m / five_denom > 0) {
      fives_unit = m / five_denom;
      m = m - (fives_unit * five_denom);
    }

    ones_unit = m; 
    m = 0;
  }

  return tens_unit + fives_unit + ones_unit;
}

int main() {
  int m;
  std::cin >> m;
  std::cout << get_change(m) << '\n';
}
