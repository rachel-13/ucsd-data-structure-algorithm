#include <iostream>
#include <cmath>

int get_fibonacci_last_digit_naive(int n) {
    if (n <= 1)
        return n;

    int previous = 0;
    int current  = 1;

    for (int i = 0; i < n - 1; ++i) {
        int tmp_previous = previous;
        previous = current;
        current = tmp_previous + current;
    }

    return current % 10;
}

int get_fibonacci_last_digit_fast(int n) {
    long long previous = 0;
    long long current = 1;
    long long upperLimit = std::pow(10, 10);

    for (int i = 2; i < n + 1; ++i) {
        long long tmp_previous = previous % upperLimit;
        previous = current % upperLimit;
        current = tmp_previous + (current % upperLimit);
    }

    return current % 10;
}

int main() {
    int n;
    std::cin >> n;
    int c = get_fibonacci_last_digit_fast(n);
    std::cout << c << '\n';
}
