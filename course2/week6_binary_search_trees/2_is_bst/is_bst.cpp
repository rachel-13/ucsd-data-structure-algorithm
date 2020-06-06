#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <stack>

using std::cin;
using std::cout;
using std::endl;
using std::stack;
using std::vector;

struct Node
{
  int key;
  int left;
  int right;

  Node() : key(0), left(-1), right(-1) {}
  Node(int key_, int left_, int right_) : key(key_), left(left_), right(right_) {}
};

bool is_sorted_by_key( const Node & lhs, const Node & rhs )
{
   return lhs.key < rhs.key;
}

bool IsBinarySearchTree(const vector<Node> &tree)
{
  // note to self: a binary search tree is a sorted binary tree and from the previous question (tree_traversal.cpp) is it seen that in-order traversal 
  // printed out the node keys in a sorted order. hence we just need to do an in-order traversal and check if the results are sorted.
  if (tree.size() == 0)
    return true;

  stack<Node> stackOfNodes;
  stack<int> stackOfIndices;

  Node root = tree[0];
  int rootIndex = 0;
  vector<Node> result;

  while (rootIndex != -1 || !stackOfNodes.empty())
  {
    if (rootIndex != -1)
    {
      stackOfNodes.push(root);
      stackOfIndices.push(rootIndex);

      Node leftChild = tree[root.left];
      rootIndex = root.left;
      root = leftChild;
    }
    else
    {
      root = stackOfNodes.top();
      rootIndex = stackOfIndices.top();
      stackOfNodes.pop();
      stackOfIndices.pop();

      result.push_back(tree[rootIndex]);

      Node rightChild = tree[root.right];
      rootIndex = root.right;
      root = rightChild;
    }
  }

  bool resultIsSorted = std::is_sorted(result.begin(), result.end(), is_sorted_by_key);
  return resultIsSorted;
}

int main()
{
  // std::fstream cin("./tests/06");

  int nodes;
  cin >> nodes;
  vector<Node> tree;
  for (int i = 0; i < nodes; ++i)
  {
    int key, left, right;
    cin >> key >> left >> right;
    tree.push_back(Node(key, left, right));
  }
  if (IsBinarySearchTree(tree))
  {
    cout << "CORRECT" << endl;
  }
  else
  {
    cout << "INCORRECT" << endl;
  }
  return 0;
}
