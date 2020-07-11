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

    ConvertGSMNetworkProblemToSat(int n, int m) : numVertices(n),
                                                  edges(m)
    {
    }

    void printEquisatisfiableSatFormula()
    {
        // guided by https://research.ijcaonline.org/encc/number1/encc004.pdf for numberOfClauses
        // and https://www.youtube.com/watch?v=HhFSgXbWiWY [Reduce 3-Colorability to SAT - Intro to Algorithms by Udacity] for number of variables


        /*
        imagine a matrix where row `i` is vertices and column `j` is color

        column j should only have 1 true value, there for in the matrix of this variables in test case 01 with 3 vertices/3 color

            c c c
        ========  
        v | 1 2 3 
        v | 4 5 6 
        v | 7 8 9 
        
        we need to create values CNF where 1 and 4 cannot be true, etc
        -1 -4 0
        -1 -7 0
        */

        int numberOfColors = 3;
        int numberOfClauses = ((numberOfColors - 2) * numVertices) + (numberOfColors * edges.size());
        int numberOfVariables = numberOfColors * numVertices;

        cout << numberOfClauses << " " << numberOfVariables << "\n";

        for(int i = 0, j = 1; i < numVertices; i++, j += numberOfColors)
        {
            cout << j << " " << j + 1 << " " << j + 2 << " 0\n";;
        }

        for(int i = 0; i < edges.size(); i++)
        {
            Edge edge = edges[i];
            cout << -((edge.from - 1) * numberOfColors + 1) << " " << -((edge.to - 1) * numberOfColors + 1) << " 0\n";
            cout << -((edge.from - 1) * numberOfColors + 2) << " " << -((edge.to - 1) * numberOfColors + 2) << " 0\n";
            cout << -((edge.from - 1) * numberOfColors + 3) << " " << -((edge.to - 1) * numberOfColors + 3) << " 0\n";
        }
    }
};

int main()
{
    ios::sync_with_stdio(false);

    fstream cin("./tests/02");

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
