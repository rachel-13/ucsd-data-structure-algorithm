#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

using std::cin;
using std::cout;
using std::make_pair;
using std::pair;
using std::swap;
using std::vector;

class HeapBuilder
{
private:
  vector<long long> data_;
  vector<pair<int, int>> swaps_;

  void WriteResponse() const
  {
    cout << swaps_.size() << "\n";
    for (int i = 0; i < swaps_.size(); ++i)
    {
      cout << swaps_[i].first << " " << swaps_[i].second << "\n";
    }
  }

  void ReadData()
  {
    // std::ifstream cin("./tests/04");

    int n;
    cin >> n;
    data_.resize(n);
    for (int i = 0; i < n; ++i)
      cin >> data_[i];
  }

  void GenerateSwaps_Naive()
  {
    swaps_.clear();
    // The following naive implementation just sorts
    // the given sequence using selection sort algorithm
    // and saves the resulting sequence of swaps.
    // This turns the given array into a heap,
    // but in the worst case gives a quadratic number of swaps.
    //
    // TODO: replace by a more efficient implementation
    for (int i = 0; i < data_.size(); ++i)
      for (int j = i + 1; j < data_.size(); ++j)
      {
        if (data_[i] > data_[j])
        {
          swap(data_[i], data_[j]);
          swaps_.push_back(make_pair(i, j));
        }
      }
  }

  void GenerateSwaps()
  {
    swaps_.clear();
    if (std::is_sorted(data_.begin(), data_.end()))
    {
      return;
    }

    int size = data_.size();
    int index = size / 2;

    for (int i = index; i >= 0; i--)
    {
      SiftDown(i);
    }
  }


  void SiftDown(int index)
  {
    int size = data_.size() - 1;
    int minIndex = index;

    int leftChildIndex = leftChild(index);
    int rightChildIndex = rightChild(index);

    if (leftChildIndex <= size && data_[leftChildIndex] < data_[minIndex])
    {
      minIndex = leftChildIndex;
    }

    if (rightChildIndex <= size && data_[rightChildIndex] < data_[minIndex])
    {
      minIndex = rightChildIndex;
    }

    if (index != minIndex)
    {
      swap(data_[index], data_[minIndex]);
      swaps_.push_back(make_pair(index, minIndex));
      SiftDown(minIndex);
    }
  }

  void SiftUp(int index)
  {
    while (index > 0 && data_[parent(index)] > data_[index])
    {
      swap(data_[index], data_[parent(index)]);
      swaps_.push_back(make_pair(parent(index), index));
      index = parent(index);
    }
  }

  int parent(int i)
  {
    return (i - 1) / 2;
  }

  int leftChild(int i)
  {
    return (2 * i) + 1;
  }

  int rightChild(int i)
  {
    return (2 * i) + 2;
  }

public:
  void Solve()
  {
    ReadData();
    GenerateSwaps();
    WriteResponse();
  }
};

int main()
{
  std::ios_base::sync_with_stdio(false);
  HeapBuilder heap_builder;
  heap_builder.Solve();
  return 0;
}
