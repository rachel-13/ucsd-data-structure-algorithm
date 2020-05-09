#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <queue>

using std::cin;
using std::cout;
using std::priority_queue;
using std::swap;
using std::vector;

class Thread
{
public:
  int id;
  long long finishTime;
};

class JobQueue
{
private:
  int num_workers_;
  vector<long long> jobs_;

  vector<int> assigned_workers_;
  vector<long long> start_times_;
  vector<Thread> threads;
  int size = -1;

  void WriteResponse() const
  {
    // cout << "assigned workers size " << assigned_workers_.size() << "\n";
    for (int i = 0; i < jobs_.size(); ++i)
    {
      cout << assigned_workers_[i] << " " << start_times_[i] << "\n";
    }
  }

  void ReadData()
  {
    // std::fstream cin("./tests/01");

    int m;
    cin >> num_workers_ >> m;
    jobs_.resize(m);

    threads = vector<Thread>(num_workers_);

    for (int i = 0; i < m; ++i)
      cin >> jobs_[i];
  }

  void AssignJobs_Naive()
  {
    // TODO: replace this code with a faster algorithm.
    assigned_workers_.resize(jobs_.size());
    start_times_.resize(jobs_.size());
    vector<long long> next_free_time(num_workers_, 0);
    for (int i = 0; i < jobs_.size(); ++i)
    {
      int duration = jobs_[i];
      int next_worker = 0;
      for (int j = 0; j < num_workers_; ++j)
      {
        if (next_free_time[j] < next_free_time[next_worker])
          next_worker = j;
      }
      assigned_workers_[i] = next_worker;
      start_times_[i] = next_free_time[next_worker];
      next_free_time[next_worker] += duration;
    }
  }

  void AssignJobs()
  {
    assigned_workers_.resize(jobs_.size());
    start_times_.resize(jobs_.size());
    for (int i = 0; i < num_workers_; i++)
    {
      Thread thread = {i, 0};
      Insert(thread);
    }

    for (int i = 0; i < jobs_.size(); i++)
    {
      long long duration = jobs_[i];
      Thread minThread = ExtractMin();

      assigned_workers_[i] = minThread.id;
      start_times_[i] = minThread.finishTime;

      minThread.finishTime = minThread.finishTime + duration;
      Insert(minThread);
    }
  }

  void Insert(Thread thread)
  {
    if (size == num_workers_)
    {
      return;
    }

    size = size + 1;
    threads[size] = thread;
    SiftUp(size);
  }

  Thread ExtractMin()
  {
    Thread result = threads[0];
    threads[0] = threads[num_workers_ - 1];
    size = size - 1;
    SiftDown(0);
    return result;
  }

  void SiftDown(long long i)
  {
    int workerMaxIndex = num_workers_ - 1;
    long long minIndex = i;

    long long leftChildIndex = leftChild(i);
    long long rightChildIndex = rightChild(i);

    if (leftChildIndex <= workerMaxIndex &&
        rightChildIndex <= workerMaxIndex &&
        threads[leftChildIndex].finishTime == threads[rightChildIndex].finishTime)
    {
      if (threads[leftChildIndex].id < threads[rightChildIndex].id)
      {
        minIndex = leftChildIndex;
      }
      else
      {
        minIndex = rightChildIndex;
      }
    }

    if (leftChildIndex <= workerMaxIndex && threads[leftChildIndex].finishTime < threads[minIndex].finishTime)
    {
      minIndex = leftChildIndex;
    }

    if (rightChildIndex <= workerMaxIndex && threads[rightChildIndex].finishTime < threads[minIndex].finishTime)
    {
      minIndex = rightChildIndex;
    }

    if (i != minIndex)
    {
      swap(threads[i], threads[minIndex]);
      SiftDown(minIndex);
    }
  }

  void SiftUp(long long i)
  {
    while (i > 0 && threads[parent(i)].finishTime > threads[i].finishTime)
    {
      swap(threads[i], threads[parent(i)]);
      i = parent(i);
    }
  }

  long long parent(long long i)
  {
    return (i - 1) / 2;
  }

  long long leftChild(long long i)
  {
    return (2 * i) + 1;
  }

  long long rightChild(long long i)
  {
    return (2 * i) + 2;
  }

  void ChangePriority(long long i, long long newFinishTime)
  {
    long long oldFinishTime = threads[i].finishTime;
    threads[i].finishTime = newFinishTime;

    if (newFinishTime > oldFinishTime)
    {
      SiftDown(i);
    }
    else
    {
      SiftUp(i);
    }
  }

public:
  void Solve()
  {
    ReadData();
    AssignJobs();
    WriteResponse();
  }
};

int main()
{
  std::ios_base::sync_with_stdio(false);
  JobQueue job_queue;
  job_queue.Solve();
  return 0;
}
