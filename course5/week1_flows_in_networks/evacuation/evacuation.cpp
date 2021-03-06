#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <limits.h>

using std::cin;
using std::min;
using std::queue;
using std::vector;
using std::pair;
using std::make_pair;

/* This class implements a bit unusual scheme for storing edges of the graph,
 * in order to retrieve the backward edge for a given edge quickly. */
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

FlowGraph read_data()
{
    // std::fstream file("./tests/36");

    // if (file.is_open())
    // {
    //     cin.rdbuf(file.rdbuf());
    // }

    int vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    FlowGraph graph(vertex_count);
    for (int i = 0; i < edge_count; ++i)
    {
        int u, v, capacity;
        std::cin >> u >> v >> capacity;
        graph.add_edge(u - 1, v - 1, capacity);
    }
    return graph;
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

int max_flow(FlowGraph &graph, int from, int to)
{
    int flow = 0;

    while (true)
    {
        vector<int> path;

        int new_flow = bfs(graph, from, to, path);

        if (new_flow == -1)
        {
            break;
        }

        flow = flow + new_flow;

        int currentNode = to;

        while (path.size() > 0)
        {
            int previousPath = path.back();
            path.pop_back();

            FlowGraph::Edge edge = graph.get_edge(previousPath);
            
            if(edge.to == currentNode)
            {
                graph.add_flow(previousPath, new_flow);
                currentNode = edge.from;
            }
        }
    }

    return flow;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    FlowGraph graph = read_data();

    std::cout << max_flow(graph, 0, graph.size() - 1) << "\n";
    return 0;
}
