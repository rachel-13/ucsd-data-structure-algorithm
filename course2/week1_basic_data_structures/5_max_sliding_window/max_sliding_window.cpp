#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>

using std::cin;
using std::cout;
using std::max;
using std::vector;

void max_sliding_window_naive(vector<int> const &A, int w)
{
    for (size_t i = 0; i < A.size() - w + 1; ++i)
    {
        int window_max = A.at(i);
        for (size_t j = i + 1; j < i + w; ++j)
            window_max = max(window_max, A.at(j));

        cout << window_max << " ";
    }

    return;
}

void max_sliding_window(vector<int> const &A, int w)
{

    if (A.size() == w)
    {
        cout << *max_element(A.begin(), A.end());
        return;
    }

    std::deque<int> q;

    // find the first window
    for(int i = 0; i < w; i++ ) {
        // remove all indexes of smaller elements than the current element
       while(!q.empty() && A[i] >= A[q.back()]) {
            q.pop_back();
        }
        q.push_back(i);
    }

    for(int k = w; k < A.size(); k++)
    {
        cout << A[q.front()] << " ";

        // remove all indexes of elements that have moved out of the window
        while(!q.empty() && q.front() <= k - w) {
            q.pop_front();
        }

        // remove all indexes of smaller elements than the current element
        while(!q.empty() && A[k] >= A[q.back()]) {
            q.pop_back();
        }

        q.push_back(k);
    }

    cout << A[q.front()];

    return;
}

void test_solution()
{
    vector<int> A;
    for (int i = 0; i <= 100000; i++)
    {
        A.push_back(0);
    }

    max_sliding_window(A, 1);
}

int main()
{
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
