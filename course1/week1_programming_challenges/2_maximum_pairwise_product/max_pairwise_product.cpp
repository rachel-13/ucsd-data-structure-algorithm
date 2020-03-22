#include <iostream>
#include <vector>
#include <algorithm>

long long MaxPairwiseProduct(const std::vector<long long>& numbers) {
    long long max_product = 0;
    int n = numbers.size();

    for (int first = 0; first < n; ++first) {
        for (int second = first + 1; second < n; ++second) {
            max_product = std::max(max_product, ((long long)numbers[first]) * numbers[second]);
        }
    }

    return max_product;
}

long long MaxPairwiseProductFast(const std::vector<long long>& numbers) {
    long long max_product = 0;
    int n = numbers.size();

    int max_index_1 = -1;
    for (int i = 0; i < n; ++i) {
        if((numbers[i] > numbers[max_index_1]) || max_index_1 == -1) {
            max_index_1 = i;
        }
    }
    
    int max_index_2 = -1;
    for (int j = 0; j < n; ++j) {
        if(numbers[j] > numbers[max_index_2] || max_index_2 == -1) {
            if(j != max_index_1) {
                max_index_2 = j;
            }
        }
    }
    
    max_product = numbers[max_index_1] * numbers[max_index_2];
    return max_product;
}

int main() {
/*
    while(true) {
        int num = rand() % 1000 + 2;

        std::vector<long long> arr;
        for (int i = 0; i < num; ++i) {
            long long random_number = rand() % 10000;
            arr.push_back(random_number);
            std::cout << random_number << " ";
        }

        std::cout << "\n";

        long long result1 = MaxPairwiseProduct(arr);
        long long result2 = MaxPairwiseProductFast(arr);

        if (result1 != result2) {
            std::cout << "Wrong value " << result1 << ", " << result2 << "\n";
            break;
        } else {
            std::cout << "OK" << "\n";
        }
    }
*/
    int n;
    std::cin >> n;
    std::vector<long long> numbers(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> numbers[i];
    }

    std::cout << MaxPairwiseProductFast(numbers) << "\n";
    return 0;
}
