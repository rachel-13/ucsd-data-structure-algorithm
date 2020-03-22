#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>

unsigned long long get_fibonacci_huge_naive(long long n, long long m) {
    if (n <= 1)
        return n;

    unsigned long long previous = 0;
    unsigned long long current  = 1;

    for (unsigned long long i = 0; i < n - 1; ++i) {
        unsigned long long tmp_previous = previous;
        previous = current;
        current = tmp_previous + current;
    }

    return current % m;
}

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

long long get_fibonacci_huge_fast(long long n, long long m) {
    long long pisano_period = get_pisano_period(m);
    long long remainder = n % pisano_period;
    long long fibonacci = get_fibonacci(remainder, m);
    long long result = fibonacci % m;
    return result;
}

void test_solution() {
    assert(get_fibonacci_huge_fast(11,3) == 2);
    assert(get_fibonacci_huge_fast(12,3) == 0);
    assert(get_fibonacci_huge_fast(5,3) == 2);
    assert(get_fibonacci_huge_fast(13,3) == 2);
    assert(get_fibonacci_huge_fast(1548276540,235) == 185);
    assert(get_fibonacci_huge_fast(2816213588,239) == 151);
    assert(get_fibonacci_huge_fast(2019,5) == 1);
    assert(get_fibonacci_huge_fast(2015,3) == 1);
    assert(get_fibonacci_huge_fast(239,1000) == 161);
    assert(get_fibonacci_huge_fast(100,999) == 858);
    assert(get_fibonacci_huge_fast(9999999999999,2) == 0);
}

int main() {
    long long n, m;
    // test_solution();
    std::cin >> n >> m;
    std::cout << get_fibonacci_huge_fast(n, m) << '\n';
}
