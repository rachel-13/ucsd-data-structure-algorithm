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
        cell i and j should only have 1 true value for each row and column, there for in the matrix of this variables in test case 01 with 3 vertices/3 color

            c c c
        ========
        v | 1 2 3
        v | 4 5 6
        v | 7 8 9

        we need to create values CNF where 1 and 4 cannot be true, etc
        -1 -4
        -1 -7
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

    void printEquisatisfiableSatFormula_alternative()
    {
        // implemented using formula obtained from Section 2.4 of this PDF https://www.ijeat.org/wp-content/uploads/papers/v6i1/A4711106116.pdf
        // it passes this minisat https://msoos.github.io/cryptominisat_web/ but not the grader due to naming scheme which churns out variables like these

        /*
            c   c   c
        ==============
        v | 11  12  13
        v | 21  22  23
        v | 31  32  33

        we need to create variables that start from 1 and consistently increment by 1 as denoted in `printEquisatisfiableSatFormula` 
        */

        int numberOfColors = 3;
        int numberOfClauses = ((numberOfColors - 2) * numVertices) + (numberOfColors * edges.size());
        int numberOfVariables = numberOfColors * numVertices;

        cout << numberOfClauses << " " << numberOfVariables << "\n";
        
        for (int i = 1; i <= numVertices; i++)
        {
            for (int j = 1; j <= numberOfColors; j++)
            {
                printf("%d%d ", i, j);
            }
            printf("0\n");
        }

        for(int j = 1; j <= numberOfColors; j++)
        {
            for(int h = 0; h < edges.size(); h++)
            {
                Edge edge = edges[h];
                printf("-%d%d -%d%d 0\n", edge.from, j, edge.to, j);
            }
        }
    }
};

int main()
{
    ios::sync_with_stdio(false);

    // fstream cin("./tests/02");

    int n, m;
    cin >> n >> m;
    ConvertGSMNetworkProblemToSat converter(n, m);
    for (int i = 0; i < m; ++i)
    {
        cin >> converter.edges[i].from >> converter.edges[i].to;
    }

    converter.printEquisatisfiableSatFormula();
    // cout << "\n====================================================\n";
    // converter.printEquisatisfiableSatFormula_alternative();

    return 0;
}
