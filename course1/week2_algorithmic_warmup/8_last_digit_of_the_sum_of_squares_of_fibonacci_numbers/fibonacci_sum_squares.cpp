#include <iostream>

long long get_fibonacci(long long n) {
    if (n <= 1) {
        return n;
    }

    unsigned long long previous = 0;
    unsigned long long current  = 1;

    for (int i = 2; i < n + 1; ++i) {
        unsigned long long tmp_previous = previous % 10;
        previous = current % 10;
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

int fibonacci_sum_squares_fast(long long n) {
    // f(n) ^ 2 = f(n) * f(n + 1)
    if (n <= 1)
        return n;
    
    long long pisano_period = get_pisano_period(10);
    long long new_n = n % pisano_period;
    long long fibo_n = get_fibonacci(new_n);
    long long fibo_n_next = get_fibonacci(new_n + 1);

    long long sum = fibo_n * fibo_n_next;

    return sum % 10;
}

int main() {
    long long n = 0;
    std::cin >> n;
    std::cout << fibonacci_sum_squares_fast(n);
}
