#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include <deque>

using std::cin;
using std::string;
using std::vector;
using std::cout;
using std::max_element;

class StackWithMaxNaive {
    vector<int> stack;

  public:

    void Push(int value) {
        stack.push_back(value);
    }

    void Pop() {
        assert(stack.size());
        stack.pop_back();
    }

    int Max() const {
        assert(stack.size());
        return *max_element(stack.begin(), stack.end());
    }
};

class StackWithMax {
    vector<int> stack;
    int maxIndex = 0;
    vector<int> maxIndexTracker;
  public:

    void Push(int value) {
        stack.push_back(value);
        if(value >= stack[maxIndex]) {
            maxIndex = stack.size() - 1;
            maxIndexTracker.push_back(maxIndex);
        }
    }

    void Pop() {
        assert(stack.size());
        if(maxIndex == stack.size() - 1) {
            maxIndexTracker.pop_back();
            maxIndex = maxIndexTracker[maxIndexTracker.size() - 1];
        }  
        stack.pop_back();
    }

    int Max() const {
        return stack[maxIndex];
    }
};

int main() {
    int num_queries = 0;
    cin >> num_queries;

    string query;
    string value;

    StackWithMax stack;

    for (int i = 0; i < num_queries; ++i) {
        cin >> query;
        if (query == "push") {
            cin >> value;
            stack.Push(std::stoi(value));
        }
        else if (query == "pop") {
            stack.Pop();
        }
        else if (query == "max") {
            cout << stack.Max() << "\n";
        }
        else {
            assert(0);
        }
    }
    return 0;
}