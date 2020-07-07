#include <ios>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

typedef vector<vector<int>> matrix;

struct Edge
{
    int from;
    int to;
};

struct ConvertGSMNetworkProblemToSat
{
    int numVertices;
    vector<Edge> edges;
    vector<int> color{1, 2, 3};

    ConvertGSMNetworkProblemToSat(int n, int m) : numVertices(n),
                                                  edges(m)
    {
    }

    void printEquisatisfiableSatFormula()
    {
        matrix table(numVertices, vector<int>(color.size(), 0));
        for (int i = 0; i < table.size(); i++)
        {
            for(int j = 0; j < table[0].size(); j++)
            {
                cout << table[i][j] << " ";
            }
            cout << "\n";
        }
    }
};

int main()
{
    ios::sync_with_stdio(false);

    fstream cin("./tests/01");

    int n, m;
    cin >> n >> m;
    ConvertGSMNetworkProblemToSat converter(n, m);
    for (int i = 0; i < m; ++i)
    {
        cin >> converter.edges[i].from >> converter.edges[i].to;
    }

    converter.printEquisatisfiableSatFormula();

    return 0;
}
