#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stack>
#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#endif

using std::cin;
using std::cout;
using std::find;
using std::ios_base;
using std::stack;
using std::vector;

class TreeOrders
{
  int n;
  vector<int> key;
  vector<int> left;
  vector<int> right;

public:
  void read()
  {
    // std::fstream cin("./tests/02");

    cin >> n;
    key.resize(n);
    left.resize(n);
    right.resize(n);
    for (int i = 0; i < n; i++)
    {
      cin >> key[i] >> left[i] >> right[i];
    }
  }

  vector<int> in_order()
  {
    vector<int> result;
    stack<int> stack;

    int rootIndex = 0;

    while (rootIndex != -1 || !stack.empty())
    {
      if (rootIndex != -1)
      {
        stack.push(rootIndex);
        int leftChildIndex = left[rootIndex];
        rootIndex = leftChildIndex;
      }
      else
      {
        rootIndex = stack.top();
        stack.pop();
        result.push_back(key[rootIndex]);

        int rightChildIndex = right[rootIndex];
        rootIndex = rightChildIndex;
      }
    }

    return result;
  }

  vector<int> pre_order()
  {
    vector<int> result;
    stack<int> stack;

    int rootIndex = 0;

    while (!stack.empty() || rootIndex != -1)
    {
      if (rootIndex != -1)
      {
        result.push_back(key[rootIndex]);
        stack.push(rootIndex);
        int leftChildIndex = left[rootIndex];
        rootIndex = leftChildIndex;
      }
      else
      {
        rootIndex = stack.top();
        stack.pop();

        int rightChildIndex = right[rootIndex];
        rootIndex = rightChildIndex;
      }
    }

    return result;
  }

  vector<int> post_order()
  {
    vector<int> result;
    stack<int> stack;

    int rootIndex = 0;
    int lastVisitedNode = -1;

    while (!stack.empty() || rootIndex != -1)
    {
      if (rootIndex != -1)
      {
        stack.push(rootIndex);
        int leftChildIndex = left[rootIndex];
        rootIndex = leftChildIndex;
      }
      else
      {
        rootIndex = stack.top();

        int rightChildIndex = right[rootIndex];

        if (rightChildIndex == -1 || lastVisitedNode == rightChildIndex)
        {
          stack.pop();  
          result.push_back(key[rootIndex]);
          lastVisitedNode = rootIndex;
          rootIndex = -1;
        }
        else
        {
          rootIndex = rightChildIndex;
        }
      }
    }

    return result;
  }
};

void print(vector<int> a)
{
  for (size_t i = 0; i < a.size(); i++)
  {
    if (i > 0)
    {
      cout << ' ';
    }
    cout << a[i];
  }
  cout << "\n";
}

int main_with_large_stack_space()
{
  ios_base::sync_with_stdio(0);
  TreeOrders t;
  t.read();
  print(t.in_order());
  print(t.pre_order());
  print(t.post_order());
  return 0;
}

int main(int argc, char **argv)
{
#if defined(__unix__) || defined(__APPLE__)
  // Allow larger stack space
  const rlim_t kStackSize = 16 * 1024 * 1024; // min stack size = 16 MB
  struct rlimit rl;
  int result;

  result = getrlimit(RLIMIT_STACK, &rl);
  if (result == 0)
  {
    if (rl.rlim_cur < kStackSize)
    {
      rl.rlim_cur = kStackSize;
      result = setrlimit(RLIMIT_STACK, &rl);
      if (result != 0)
      {
        std::cerr << "setrlimit returned result = " << result << "\n";
      }
    }
  }
#endif

  return main_with_large_stack_space();
}
