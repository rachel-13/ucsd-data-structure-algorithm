#include <algorithm>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;

bool isGreaterOrEqual(string a, string b) {
  string ab = a + b;
  string ba = b + a;

  int ab_int; 
  std::stringstream ab_s(ab);
  ab_s >> ab_int;

  int ba_int; 
  std::stringstream ba_s(ba);
  ba_s >> ba_int;

  if(ab_int > ba_int) {
    return true;
  } else {
    return false;
  }
}

string largest_number(vector<string> digits) {
  //write your code here
  std::stringstream ret;
  while(digits.size() > 0) {
    string maxDigit = "0";

    for (size_t i = 0; i < digits.size(); i++) {
      string digit = digits[i];
      if(isGreaterOrEqual(digit, maxDigit)) {
        maxDigit = digit;
      }
    }

    ret << maxDigit;

    // removing max digit string
    std::vector<string>::iterator position = std::find(digits.begin(), digits.end(), maxDigit);
    if (position != digits.end()) {
      digits.erase(position);
    }
  }

  string result;
  ret >> result;
  return result;
}

int main() {
  int n;
  std::cin >> n;
  vector<string> a(n);
  for (size_t i = 0; i < a.size(); i++) {
    std::cin >> a[i];
  }
  std::cout << largest_number(a);
}
