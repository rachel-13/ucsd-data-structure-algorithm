#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <fstream>
#include <stack>
#include <unordered_map>
#include <set>
#include <sys/resource.h>
using namespace std;

struct Clause
{
    int firstVar;
    int secondVar;
};

struct Edge
{
public:
    int from, to;
};

struct Vertex
{
    int literal;
    int index;
    int lowLink;
    bool onStack;
    vector<Edge> edges;
};

typedef vector<vector<int>> matrix;

struct TwoSatisfiability
{
public:
    int numVars;
    vector<Clause> clauses;
    vector<Edge> edges;
    int adjListSize = 2 * numVars;
    matrix adjList;
    vector<int> mapIndexToLiteral;
    matrix scc;

    TwoSatisfiability(int n, int m) : numVars(n),
                                      clauses(m)
    {
    }

    void buildAdjList()
    {
        adjList = matrix(adjListSize);
        mapIndexToLiteral = vector<int>(adjListSize);

        for (int i = 0; i < clauses.size(); i++)
        {
            Edge edge1 = Edge();
            edge1.from = -(clauses[i].firstVar);
            edge1.to = clauses[i].secondVar;

            Edge edge2 = Edge();
            edge2.from = -(clauses[i].secondVar);
            edge2.to = clauses[i].firstVar;

            edges.push_back(edge1);
            edges.push_back(edge2);

            int adjListFrom1 = (edge1.from - 1) * 2;
            if (edge1.from < 0)
            {
                adjListFrom1 = (abs(edge1.from) * 2) - 1;
            }

            int adjListTo1 = (edge1.to - 1) * 2;
            if (edge1.to < 0)
            {
                adjListTo1 = (abs(edge1.to) * 2) - 1;
            }

            int adjListFrom2 = (edge2.from - 1) * 2;
            if (edge2.from < 0)
            {
                adjListFrom2 = (abs(edge2.from) * 2) - 1;
            }

            int adjListTo2 = (edge2.to - 1) * 2;
            if (edge2.to < 0)
            {
                adjListTo2 = (abs(edge2.to) * 2) - 1;
            }

            mapIndexToLiteral[adjListFrom1] = edge1.from;
            mapIndexToLiteral[adjListTo1] = edge1.to;
            mapIndexToLiteral[adjListFrom2] = edge2.from;
            mapIndexToLiteral[adjListTo2] = edge2.to;

            adjList[adjListFrom1].push_back(adjListTo1);
            adjList[adjListFrom2].push_back(adjListTo2);
        }
    }

    void findComponentSets(int u, int disc[], int low[], stack<int> &stk, bool stkItem[])
    {
        static int time = 0;
        disc[u] = low[u] = ++time; //inilially discovery time and low value is 1
        stk.push(u);
        stkItem[u] = true; //flag as u in the stack

        for (int v = 0; v < adjList[u].size(); v++)
        {
            int ind = adjList[u][v];
            if (disc[ind] == -1)
            { //when v is not visited
                findComponentSets(ind, disc, low, stk, stkItem);
                low[u] = min(low[u], low[ind]);
            }
            else if (stkItem[v]) //when v is in the stack, update low for u
                low[u] = min(low[u], disc[ind]);
        }

        int poppedItem = 0;
        if (low[u] == disc[u])
        {
            vector<int> v;
            while (stk.top() != u)
            {

                poppedItem = stk.top();
                int literal = mapIndexToLiteral[poppedItem];
                // cout << literal << " ";
                v.push_back(literal);

                stkItem[poppedItem] = false; //mark as item is popped
                stk.pop();
            }

            poppedItem = stk.top();
            int literal = mapIndexToLiteral[poppedItem];
            // cout << literal << endl;
            v.push_back(literal);

            stkItem[poppedItem] = false;
            stk.pop();

            scc.push_back(v);
        }
    }

    void searchAllScc()
    {
        int disc[adjListSize], low[adjListSize];
        bool stkItem[adjListSize];
        stack<int> stk;

        for (int i = 0; i < adjListSize; i++)
        { //initialize all elements
            disc[i] = low[i] = -1;
            stkItem[i] = false;
        }

        for (int i = 0; i < adjListSize; i++) //initialize all elements
            if (disc[i] == -1)
            {
                findComponentSets(i, disc, low, stk, stkItem);
            }
    }

    bool isSatisfiable(vector<int> &result)
    {
        buildAdjList();
        searchAllScc();

        for (int i = 0; i < scc.size(); i++)
        {
            vector<int> component = scc[i];

            for (int j = 0; j < numVars; j++)
            {
                int variable = j + 1;
                int variableNegation = variable * -1;
                auto it1 = find(component.begin(), component.end(), variable);
                auto it2 = find(component.begin(), component.end(), variableNegation);
                if (it1 != component.end() && it2 != component.end())
                {
                    return false;
                }
            }

            for (int j = 0; j < component.size(); j++)
            {
                int literal = component[j];
                if (result[abs(literal) - 1] == 9999999)
                {
                    result[abs(literal) - 1] = literal;
                }
            }
        }

        return true;
    }
};

int main()
{
    ios::sync_with_stdio(false);

    // fstream cin("./tests/02");

    int n, m;
    cin >> n >> m;
    TwoSatisfiability twoSat(n, m);
    for (int i = 0; i < m; ++i)
    {
        cin >> twoSat.clauses[i].firstVar >> twoSat.clauses[i].secondVar;
    }

    vector<int> result(n, 9999999);
    if (twoSat.isSatisfiable(result))
    {
        cout << "SATISFIABLE" << endl;
        for (int i = 0; i < n; ++i)
        {
            cout << result[i] << " ";
        }
    }
    else
    {
        cout << "UNSATISFIABLE" << endl;
    }

    return 0;
}
