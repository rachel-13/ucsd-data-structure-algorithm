#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <fstream>
#include <limits.h>
#include <queue>

using std::cin;
using std::cout;
using std::min;
using std::pair;
using std::queue;
using std::vector;

class FlowGraph
{
public:
  struct Edge
  {
    int from, to, capacity, flow;
  };

private:
  /* List of all - forward and backward - edges */
  vector<Edge> edges;

  /* These adjacency lists store only indices of edges in the edges list */
  vector<vector<size_t>> graph;

public:
  explicit FlowGraph(size_t n) : graph(n) {}

  void add_edge(int from, int to, int capacity)
  {
    /* Note that we first append a forward edge and then a backward edge,
         * so all forward edges are stored at even indices (starting from 0),
         * whereas backward edges are stored at odd indices in the list edges */
    Edge forward_edge = {from, to, capacity, 0};
    Edge backward_edge = {to, from, 0, 0};
    graph[from].push_back(edges.size());
    edges.push_back(forward_edge);
    graph[to].push_back(edges.size());
    edges.push_back(backward_edge);
  }

  size_t size() const
  {
    return graph.size();
  }

  const vector<size_t> &get_ids(int from) const
  {
    return graph[from];
  }

  const Edge &get_edge(size_t id) const
  {
    return edges[id];
  }

  void add_flow(size_t id, int flow)
  {
    /* To get a backward edge for a true forward edge (i.e id is even), we should get id + 1
         * due to the described above scheme. On the other hand, when we have to get a "backward"
         * edge for a backward edge (i.e. get a forward edge for backward - id is odd), id - 1
         * should be taken.
         *
         * It turns out that id ^ 1 works for both cases. Think this through! */
    edges[id].flow += flow;
    edges[id ^ 1].flow -= flow;

    edges[id].capacity -= flow;
    edges[id ^ 1].capacity += flow;
  }
};

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
    // std::fstream cin("./tests/02");

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
        cout << (matching[i] + 1);
    }
    cout << "\n";
  }

  int bfs(FlowGraph &graph, int from, int to, vector<int> &path)
  {
    int smallestCapacity = INT_MAX;
    queue<int> q;
    vector<bool> visited(graph.size());

    visited[from] = true;
    q.push(from);

    while (!q.empty())
    {
      int currentNodeIndex = q.front();
      q.pop();

      vector<size_t> edgesOfStartNode = graph.get_ids(currentNodeIndex);

      for (int i = 0; i < edgesOfStartNode.size(); i++)
      {
        FlowGraph::Edge edge = graph.get_edge(edgesOfStartNode[i]);
        if (edge.capacity > 0 && visited[edge.to] == false)
        {
          path.push_back(edgesOfStartNode[i]);
          visited[edge.to] = true;
          smallestCapacity = min(smallestCapacity, edge.capacity);

          if (edge.to == to)
          {
            return smallestCapacity;
          }

          q.push(edge.to);
        }
      }
    }

    return -1;
  }

  vector<int> FindMatching(const vector<vector<bool>> &adj_matrix)
  {
    vector<int> matching(adj_matrix.size(), -1);

    int matrixSize = adj_matrix[0].size() + adj_matrix.size() + 2; // number of col + number of row + source + sink
    FlowGraph graph(matrixSize);

    // build graph from source to nodes
    for (int j = 1; j <= adj_matrix.size(); j++)
    {
      int from = 0;
      int to = j;
      graph.add_edge(from, to, 1);
    }

    // build graph from sink to nodes
    // number of nodes on left side of the bipartite graph + source node is the starting index for node on right side of bipartite graph
    int startingColIndexToMap = adj_matrix.size() + 1;
    for (int i = startingColIndexToMap; i < matrixSize; i++)
    {
      int from = i;
      int to = matrixSize - 1;
      graph.add_edge(from, to, 1);
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
          graph.add_edge(from, to, 1);
        }
      }
    }

    while (true)
    {
      vector<int> path;

      int new_flow = bfs(graph, 0, matrixSize - 1, path);

      if (new_flow == -1)
      {
        break;
      }

      int currentNode = matrixSize - 1;

      while (path.size() > 0)
      {
        int previousPath = path.back();
        path.pop_back();

        FlowGraph::Edge edge = graph.get_edge(previousPath);

        if (edge.to == currentNode)
        {
          if(edge.from < startingColIndexToMap && edge.to >= startingColIndexToMap && edge.capacity == 1)
          {
            matching[edge.from - 1] = edge.to - startingColIndexToMap;
          }

          graph.add_flow(previousPath, new_flow);
          currentNode = edge.from;
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