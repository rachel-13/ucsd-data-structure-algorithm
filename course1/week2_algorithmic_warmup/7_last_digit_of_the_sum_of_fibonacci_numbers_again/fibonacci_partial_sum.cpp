#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>

using std::vector;

long long get_fibonacci(long long n, long long m) {
    if (n <= 1) {
        return n;
    }

    unsigned long long previous = 0;
    unsigned long long current  = 1;

    for (int i = 2; i < n + 1; ++i) {
        unsigned long long tmp_previous = previous % m;
        previous = current % m;
        current = tmp_previous + current;
    }
    return current;
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
        long long tmp_previous = previous % 100;
        previous = current % 100;
        current = tmp_previous + current;
    }

    return (current - 1);
}

long long get_fibonacci_partial_sum_fast(long long from, long long to) {
    if (to <= 1) 
        return to;

    if (from > 0) {
        from = from - 1;
    }
    
    long long diff = 0;
    long long from_pisano = from % get_pisano_period(10);
    long long to_pisano = to % get_pisano_period(10);

    long long from_sum = fibonacci_sum_fast(from);
    long long to_sum = fibonacci_sum_fast(to);

    diff = to_sum - from_sum;
    diff = (diff % 10) + 10;

    return diff % 10;
}

void test_solution() {
    assert(get_fibonacci_partial_sum_fast(0,7) == 3);
    assert(get_fibonacci_partial_sum_fast(0,0) == 0);
    assert(get_fibonacci_partial_sum_fast(1,3) == 4);
    assert(get_fibonacci_partial_sum_fast(3,7) == 1);
    assert(get_fibonacci_partial_sum_fast(10,10) == 5);
    assert(get_fibonacci_partial_sum_fast(10,200) == 2);
    assert(get_fibonacci_partial_sum_fast(1234, 12345) == 8);
    assert(get_fibonacci_partial_sum_fast(5618252, 6583591534156) == 6);
}

int main() {
    long long from, to;
    // test_solution();
    std::cin >> from >> to;
    std::cout << get_fibonacci_partial_sum_fast(from, to) << '\n';
}
