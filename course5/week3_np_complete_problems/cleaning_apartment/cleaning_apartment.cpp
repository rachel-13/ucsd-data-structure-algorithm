#include <iostream>
#include <vector>
#include <cstdio>
#include <fstream>
#include <string>

using namespace std;

struct Edge
{
    int from;
    int to;
};

struct ConvertHampathToSat
{
    int numVertices;
    vector<Edge> edges;
    vector<string> clausesStrArr;
    typedef vector<vector<int>> matrix;
    int numberOfVariables = numVertices * numVertices;

    ConvertHampathToSat(int n, int m) : numVertices(n),
                                        edges(m)
    {
    }

    void printEquisatisfiableSatFormula()
    {
        // guided by https://www.csie.ntu.edu.tw/~lyuu/complexity/2011/20111018.pdf
        // i = vertex/node
        // j = position of the node in the path
        matrix adjacency(numVertices, vector<int>(numVertices));
        for (int i = 0; i < edges.size(); i++)
        {
            Edge edge = edges[i];
            int from = edge.from - 1;
            int to = edge.to - 1;
            adjacency[from][to] = 1;
            adjacency[to][from] = 1;
        }

        // build variable naming scheme
        matrix variableMap(numVertices, vector<int>(numVertices, 0));
        int variableCount = 1;
        for (int i = 0; i < numVertices; i++)
        {
            for (int j = 0; j < numVertices; j++)
            {
                variableMap[i][j] = variableCount;
                variableCount++;
            }
        }

        everyNodeMustAppearOnAPosition(variableMap);
        noNodeAppearsTwiceInAPath(variableMap);
        everyPositionMustBeOccupied(variableMap);
        noTwoNodesOccupyTheSamePosition(variableMap);
        nonadjacentNodesCannotHaveAdjacentPath(adjacency, variableMap);

        // print results
        printf("%d %d\n", clausesStrArr.size(), numberOfVariables);
        for (int i = 0; i < clausesStrArr.size(); i++)
        {
            string clause = clausesStrArr[i];
            printf("%s", clause.c_str());
        }
    }

    void everyNodeMustAppearOnAPosition(matrix variableMap)
    {
        // every node must appear on a path or position
        for (int i = 0; i < numVertices; i++)
        {
            string clause = "";
            for (int j = 0; j < numVertices; j++)
            {
                clause = clause + to_string(variableMap[i][j]);
                clause = clause + " ";
            }
            clause = clause + "0\n";
            clausesStrArr.push_back(clause);
        }
    }

    void noNodeAppearsTwiceInAPath(matrix variableMap)
    {
        // no vertex/node appears twice in the path
        for (int i = 0; i < numVertices; i++)
        {
            for (int j = 0; j < numVertices; j++)
            {
                for (int k = i + 1; k < numVertices; k++)
                {
                    string clause = "";
                    clause = clause + to_string(-variableMap[i][j]) + " " + to_string(-variableMap[k][j]) + " 0\n";
                    clausesStrArr.push_back(clause);
                }
            }
        }
    }

    void everyPositionMustBeOccupied(matrix variableMap)
    {
        // every position must be occupied
        for (int i = 0; i < numVertices; i++)
        {
            string clause = "";
            for (int j = 0; j < numVertices; j++)
            {
                clause = clause + to_string(variableMap[j][i]);
                clause = clause + " ";
            }
            clause = clause + "0\n";
            clausesStrArr.push_back(clause);
        }
    }

    void noTwoNodesOccupyTheSamePosition(matrix variableMap)
    {
        // no 2 vertices occupy the same position in the path
        for (int i = 0; i < numVertices; i++)
        {
            for (int j = 0; j < numVertices; j++)
            {
                for (int k = j + 1; k < numVertices; k++)
                {
                    string clause = "";
                    clause = clause + to_string(-variableMap[i][j]) + " " + to_string(-variableMap[i][k]) + " 0\n";
                    clausesStrArr.push_back(clause);
                }
            }
        }
    }

    void nonadjacentNodesCannotHaveAdjacentPath(matrix adjacency, matrix variableMap)
    {
        // nonadjacent nodes cannot be adjacent in path
        /*
            this means that for the following graph and matrix

            1 -- 2 -- 3 -- 5 -- 4

                1   2   3   4   5   <-- position
            =====================
            1|  1   2   3   4   5   <-- node 1
            2|  6   7   8   9   10
            3|  11  12  13  14  15
            4|  16  17  18  19  20
            5|  21  22  23  24  25

            we cannot jump from node 1 to node 3 in the path because they are no connected by an edge. 
            in other words, if node 1 is at position 1 of the path, node 3 will never be in position 2.
            if node 1 is at position 2 of the path, node 3 can never be in position 1 or 3 in the path.
            therefore we will need to make clause like (-1 V -7)(-2 V -13) etc

        */
        for (int i = 0; i < numVertices; i++)
        {
            for (int j = 0; j < numVertices; j++)
            {
                if (adjacency[i][j] == 0 && i != j)
                {
                    for (int k = 0; k < numVertices - 1; k++)
                    {
                        string clause = "";
                        clause = clause + to_string(-variableMap[i][k]) + " " + to_string(-variableMap[j][k + 1]) + " 0\n";
                        clausesStrArr.push_back(clause);
                    }
                }
            }
        }
    }
};

int main()
{
    ios::sync_with_stdio(false);

    // fstream cin("./tests/03");

    int n, m;
    cin >> n >> m;
    ConvertHampathToSat converter(n, m);
    for (int i = 0; i < m; ++i)
    {
        cin >> converter.edges[i].from >> converter.edges[i].to;
    }

    converter.printEquisatisfiableSatFormula();

    return 0;
}
