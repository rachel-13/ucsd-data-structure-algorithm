#include <iostream>
#include <vector>
#include <fstream>

using namespace std;
struct Vertex
{
    int weight;
    std::vector<int> children;
};
typedef std::vector<Vertex> Graph;
typedef std::vector<int> Sum;

Graph ReadTree()
{
    int vertices_count;

    // std::fstream file("./tests/03");

    // if (file.is_open())
    // {
    //     std::cin.rdbuf(file.rdbuf());
    // }
    std::cin >> vertices_count;

    Graph tree(vertices_count);

    for (int i = 0; i < vertices_count; ++i)
        std::cin >> tree[i].weight;

    for (int i = 1; i < vertices_count; ++i)
    {
        int from, to, weight;
        std::cin >> from >> to;
        tree[from - 1].children.push_back(to - 1);
        tree[to - 1].children.push_back(from - 1);
    }

    return tree;
}

int dfs(const Graph &tree, int vertex, int parent, vector<int> &maxWeightArr, vector<bool> &visitedNodes)
{
    if (maxWeightArr[vertex] == -1)
    {
        if (tree[vertex].children.size() == 0)
        {
            maxWeightArr[vertex] = tree[vertex].weight;
        }
        else
        {
            int m1 = tree[vertex].weight;
            for (int i = 0; i < tree[vertex].children.size(); i++)
            {
                int child = tree[vertex].children[i];
                if(child == parent)
                {
                    continue;
                }
                for (int grandchild : tree[child].children)
                {
                    if(grandchild != vertex)
                    {
                        m1 = m1 + dfs(tree, grandchild, child, maxWeightArr, visitedNodes);
                    }
                }
            }

            int m0 = 0;
            for (int i = 0; i < tree[vertex].children.size(); i++)
            {   
                int child = tree[vertex].children[i];
                if(child == parent)
                {
                    continue;
                }
                m0 = m0 + dfs(tree, child, vertex, maxWeightArr, visitedNodes);
            }

            maxWeightArr[vertex] = max(m0,m1);
        }
    }

    return maxWeightArr[vertex];
}

int MaxWeightIndependentTreeSubset(const Graph &tree)
{
    size_t size = tree.size();
    if (size == 0)
    {
        return 0;
    }

    vector<int> maxWeightArr(tree.size(), -1);
    vector<bool> visitedNodes(tree.size(), false);

    return dfs(tree, 0, -1, maxWeightArr, visitedNodes);;
}

int main()
{
    Graph tree = ReadTree();
    int weight = MaxWeightIndependentTreeSubset(tree);
    std::cout << weight << std::endl;
    return 0;
}
