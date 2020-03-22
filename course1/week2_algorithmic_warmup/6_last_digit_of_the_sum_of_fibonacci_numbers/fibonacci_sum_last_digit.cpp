#include <iostream>
#include <cassert>
#include <cmath>

int fibonacci_sum_naive(long long n) {
    if (n <= 1)
        return n;

    long long previous = 0;
    long long current  = 1;
    long long sum      = 1;

    for (long long i = 0; i < n - 1; ++i) {
        long long tmp_previous = previous;
        previous = current;
        current = tmp_previous + current;
        sum += current;
    }

    return sum % 10;
}

long long get_pisano_period(long long m) {
    long long previous = 0;
    long long current  = 1;
    
    for (int i = 0; i < m*m; ++i) {
        long long tmp_previous = previous;
        previous = current;
        current = (tmp_previous + current) % m;

        if (previous == 0 && current == 1) {
            return i + 1;
        }
    }
}

int fibonacci_sum_fast(long long n) {

    if (n <= 1)
        return n;

    long long pisano_period = get_pisano_period(10);
    long long remainder = n % pisano_period;

    long long previous = 0;
    long long current  = 1;

    for (long long i = 2; i < remainder + 3; ++i) {
        long long tmp_previous = previous;
        previous = current;
        current = tmp_previous + current;
    }

    return (current - 1) % 10;
}

void test_solution() {
    assert(fibonacci_sum_fast(3) == 4);
    assert(fibonacci_sum_fast(100) == 5);
}

int main() {
    long long n = 0;
    std::cin >> n;
    std::cout << fibonacci_sum_fast(n);
}
