#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>  

using std::cin;
using std::cout;
using std::vector;
using std::max;

void max_sliding_window_naive(vector<int> const & A, int w) {
    for (size_t i = 0; i < A.size() - w + 1; ++i) {
        int window_max = A.at(i);
        for (size_t j = i + 1; j < i + w; ++j)
            window_max = max(window_max, A.at(j));

        cout << window_max << " ";
    }

    return;
}

void max_sliding_window(vector<int> const & A, int w) {

    if(A.size() == w) {
        cout<< *max_element(A.begin(), A.end());
        return;
    }

    std::deque<int> q;
    int max = -1;
    int index = 0;

    while(index < A.size()) {
        
       if(q.size() < w) 
       {  
           q.push_back(A[index]);
       } 
       else 
       {
            q.pop_front();
            q.push_back(A[index]);
       }

       index++;

       if(index >= w) 
       {
           max = *max_element(q.begin(), q.end());
           cout << max << " ";
       }
    }

    return;
}

void test_solution() {
    vector<int> A;
    for(int i = 0; i <= 100000; i++) {
        A.push_back(0);
    }

    max_sliding_window(A, 1);
}

int main() {
    // test_solution();

    int n = 0;
    cin >> n;

    vector<int> A(n);
    for (size_t i = 0; i < n; ++i)
        cin >> A.at(i);

    int w = 0;
    cin >> w;

    max_sliding_window(A, w);

    return 0;
}
