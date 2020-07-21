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
    vector<int> literalMappedToSCC;
    int adjListSize = 2 * numVars;
    matrix adjList;
    vector<int> mapIndexToLiteral;
    matrix scc;
    int sccCount = 0;

    TwoSatisfiability(int n, int m) : numVars(n),
                                      clauses(m)
    {
    }

    void buildAdjList()
    {
        adjList = matrix(adjListSize);
        mapIndexToLiteral = vector<int>(adjListSize);
        literalMappedToSCC = vector<int>(adjListSize, -1);

        for (int i = 0; i < clauses.size(); i++)
        {
            Edge edge1 = Edge();
            edge1.from = -(clauses[i].firstVar);
            edge1.to = clauses[i].secondVar;

            Edge edge2 = Edge();
            edge2.from = -(clauses[i].secondVar);
            edge2.to = clauses[i].firstVar;

            int adjListFrom1 = edge1.from - 1;
            if (edge1.from < 0)
            {
                adjListFrom1 = (abs(edge1.from) - 1) + numVars;
            }

            int adjListTo1 = edge1.to - 1;
            if (edge1.to < 0)
            {
                adjListTo1 = (abs(edge1.to) - 1) + numVars;
            }

            int adjListFrom2 = edge2.from - 1;
            if (edge2.from < 0)
            {
                adjListFrom2 = (abs(edge2.from) - 1) + numVars;
            }

            int adjListTo2 = edge2.to - 1;
            if (edge2.to < 0)
            {
                adjListTo2 = (abs(edge2.to) - 1) + numVars;
            }

            mapIndexToLiteral[adjListFrom1] = edge1.from;
            mapIndexToLiteral[adjListTo1] = edge1.to;
            mapIndexToLiteral[adjListFrom2] = edge2.from;
            mapIndexToLiteral[adjListTo2] = edge2.to;

            adjList[adjListFrom1].push_back(adjListTo1);
            adjList[adjListFrom2].push_back(adjListTo2);
        }
    }

    void findComponentSets(int u, vector<int> &disc, vector<int> &low, stack<int> &stk, vector<bool> &stkItem)
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
            else if (stkItem[ind]) //when v is in the stack, update low for u
                low[u] = min(low[u], disc[ind]);
        }

        
        int poppedItem = 0;
        if (low[u] == disc[u])
        {
            ++sccCount;
            vector<int> v;
            while (stk.top() != u)
            {

                poppedItem = stk.top();
                int literal = mapIndexToLiteral[poppedItem];
                // cout << literal << " ";
                v.push_back(literal);
                literalMappedToSCC[poppedItem] = sccCount;

                stkItem[poppedItem] = false; //mark as item is popped
                stk.pop();
            }

            poppedItem = stk.top();
            int literal = mapIndexToLiteral[poppedItem];
            // cout << literal << endl;
            v.push_back(literal);
            literalMappedToSCC[poppedItem] = sccCount;

            stkItem[poppedItem] = false;
            stk.pop();

            scc.push_back(v);
        }
    }

    void searchAllScc()
    {
        vector<int> disc(adjListSize, -1);
        vector<int> low(adjListSize, -1);
        vector<bool> stkItem(adjListSize, false);
        stack<int> stk;

        for (int i = 0; i < adjListSize; i++) //initialize all elements
        {
            if (disc[i] == -1)
            {
                findComponentSets(i, disc, low, stk, stkItem);
            }
        }
    }

    bool isSatisfiable(vector<int> &result)
    {
        buildAdjList();
        searchAllScc();

        for(int i = 0; i < numVars; i++)
        {
            if(literalMappedToSCC[i] == literalMappedToSCC[i+numVars])
            {
                return false;
            }
        }

        for (int i = 0; i < scc.size(); i++)
        {
            vector<int> component = scc[i];
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
    // This code is here to increase the stack size to avoid stack overflow
    // in depth-first search.
    const rlim_t kStackSize = 64L * 1024L * 1024L;  // min stack size = 64 Mb
    struct rlimit rl;
    int sresult;
    sresult = getrlimit(RLIMIT_STACK, &rl);
    if (sresult == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            sresult = setrlimit(RLIMIT_STACK, &rl);
            if (sresult != 0)
            {
                fprintf(stderr, "setrlimit returned result = %d\n", sresult);
            }
        }
    }

    ios::sync_with_stdio(false);

    // fstream cin("./tests/03");

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
