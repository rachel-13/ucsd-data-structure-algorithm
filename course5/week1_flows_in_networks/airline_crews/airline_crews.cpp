#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <fstream>
#include <limits.h>
#include <queue>

using std::cin;
using std::cout;
using std::pair;
using std::queue;
using std::vector;

class MaxMatching
{
public:
  void Solve()
  {
    vector<vector<bool>> adj_matrix = ReadData();
    vector<int> matching = FindMatching(adj_matrix);
    WriteResponse(matching);
  }

private:
  vector<vector<bool>> ReadData()
  {
    std::fstream cin("./tests/02");

    int num_left, num_right;
    cin >> num_left >> num_right;
    vector<vector<bool>> adj_matrix(num_left, vector<bool>(num_right));
    for (int i = 0; i < num_left; ++i)
      for (int j = 0; j < num_right; ++j)
      {
        int bit;
        cin >> bit;
        adj_matrix[i][j] = (bit == 1);
      }
    return adj_matrix;
  }

  void WriteResponse(const vector<int> &matching)
  {
    for (int i = 0; i < matching.size(); ++i)
    {
      if (i > 0)
        cout << " ";
      if (matching[i] == -1)
        cout << "-1";
      else
        cout << (matching[i]);
    }
    cout << "\n";
  }

  int bfs(int source, int sink, vector<vector<int>> &graph, vector<vector<int>> &capacity, vector<int> &path)
  {
    fill(path.begin(), path.end(), -1);
    path[source] = -2;
    queue<pair<int, int>> q;
    q.push({source, INT_MAX});

    while (!q.empty())
    {
      int cur = q.front().first;
      int flow = q.front().second;
      q.pop();

      for (int next : graph[cur])
      {
        if (path[next] == -1 && capacity[cur][next])
        {
          path[next] = cur;
          int new_flow = std::min(flow, capacity[cur][next]);
          if (next == sink)
            return new_flow;
          q.push({next, new_flow});
        }
      }
    }

    return 0;
  }

  vector<int> FindMatching(const vector<vector<bool>> &adj_matrix)
  {
    vector<int> matching(adj_matrix.size(), -1);

    int matrixSize = adj_matrix[0].size() + adj_matrix.size() + 2; // number of col + number of row + source + sink
    vector<vector<int>> capacity(matrixSize, vector<int>(matrixSize));
    vector<vector<int>> graph(matrixSize);

    // build graph from source to nodes
    for (int j = 1; j <= adj_matrix.size(); j++)
    {
      int from = 0;
      int to = j;
      capacity[from][to] = 1;
      graph[from].push_back(to);
      graph[to].push_back(from);
    }

    // build graph from sink to nodes
    // number of nodes on left side of the bipartite graph + source node is the starting index for node on right side of bipartite graph
    int startingColIndexToMap = adj_matrix.size() + 1;
    for (int i = startingColIndexToMap; i < capacity.size() - 1; i++)
    {
      int from = i;
      int to = capacity[0].size() - 1;
      capacity[from][to] = 1;
      graph[from].push_back(to);
      graph[to].push_back(from);
    }

    // build graph for intermediate values based on input
    for (int i = 0; i < adj_matrix.size(); i++)
    {
      for (int j = 0; j < adj_matrix[0].size(); j++)
      {
        int from = i + 1; // offset source node
        int to = j + startingColIndexToMap;
        int originalFrom = i;
        int originalTo = j;
        if (adj_matrix[originalFrom][originalTo] == 1)
        {
          capacity[from][to] = 1;
          graph[from].push_back(to);
          graph[to].push_back(from);
        }
      }
    }

    int flow = 0;
    vector<int> path(matrixSize);
    int new_flow;
    int source = 0;
    int to = matrixSize - 1;

    while (new_flow = bfs(0, to, graph, capacity, path))
    {
      flow += new_flow;
      int cur = to;
      while (cur != source)
      {
        int prev = path[cur];
        capacity[prev][cur] -= new_flow;
        capacity[cur][prev] += new_flow;
        cur = prev;
      }
    }

    for (int i = startingColIndexToMap; i < capacity.size() - 1; i++)
    {
      for (int j = 0; j < adj_matrix[0].size(); j++)
      {
        // j starts at 1 because 0 is source node but we are not interested in edges between source/sink and other nodes
        // we are only interested in edged between 2 sides of the bipartite nodes
        if (capacity[i][j + 1] == 1)
        {
          matching[i - startingColIndexToMap] = j + 1;
        }
      }
    }

    return matching;
  }
};

int main()
{
  std::ios_base::sync_with_stdio(false);
  MaxMatching max_matching;
  max_matching.Solve();
  return 0;
}