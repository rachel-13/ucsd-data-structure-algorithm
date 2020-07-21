#include <sys/resource.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <fstream>
#include <stack>
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
    // matrix scc;
    int sccID = 0;
    std::vector<bool> sol;

    TwoSatisfiability(int n, int m) : numVars(n),
                                      clauses(m)
    {
    }

    void buildAdjList()
    {
        adjList = matrix(adjListSize);
        mapIndexToLiteral = vector<int>(adjListSize);
        literalMappedToSCC = vector<int>(adjListSize, -1);
        sol = vector<bool>(numVars);

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

    void findComponentSets(int u, matrix &adjList, vector<int> &disc, vector<int> &low, stack<int> &stk, vector<bool> &stkItem)
    {
        static int time = 0;
        disc[u] = low[u] = ++time; //inilially discovery time and low value is 1
        stk.push(u);
        stkItem[u] = true; //flag as u in the stack

        for (int i = 0; i < adjList[u].size(); i++)
        {
            auto& v = adjList[u][i];
            if (disc[v] == -1)
            { //when v is not visited
                findComponentSets(v, adjList, disc, low, stk, stkItem);
                low[u] = min(low[u], low[v]);
            }
            else if (stkItem[v]) //when v is in the stack, update low for u
                low[u] = min(low[u], disc[v]);
        }

        if (low[u] == disc[u])
        {
            ++sccID;
            while (stk.top() != u)
            {
                int poppedItem = stk.top();
                int literal = mapIndexToLiteral[poppedItem];
                // cout << literal << " ";
                literalMappedToSCC[poppedItem] = sccID;

                stkItem[poppedItem] = false; //mark as item is popped
                stk.pop();

                if (poppedItem > numVars)
                {
                    if (literalMappedToSCC[poppedItem] == literalMappedToSCC[poppedItem - numVars])
                    {
                        sat = false;
                        return;
                    }
                }
                else
                {
                    if (literalMappedToSCC[poppedItem] == literalMappedToSCC[poppedItem + numVars])
                    {
                        sat = false;
                        return;
                    }
                }
                
                sol[abs(literal) - 1] = literal < 0 ? true : false;
            }

            int poppedItem = stk.top();
            int literal = mapIndexToLiteral[poppedItem];
            // cout << literal << endl;
            literalMappedToSCC[poppedItem] = sccID;

            stkItem[poppedItem] = false;
            stk.pop();

            if (poppedItem > numVars)
            {
                if (literalMappedToSCC[poppedItem] == literalMappedToSCC[poppedItem - numVars])
                {
                    sat = false;
                    return;
                }
            }
            else
            {
                if (literalMappedToSCC[poppedItem] == literalMappedToSCC[poppedItem + numVars])
                {
                    sat = false;
                    return;
                }
            }
            sol[abs(literal) - 1] = literal < 0 ? true : false;
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
                findComponentSets(i, adjList, disc, low, stk, stkItem);
            }
        }
    }

    bool isSatisfiable(vector<int> &result)
    {
        buildAdjList();
        searchAllScc();

        if (!sat)
        {
            return false;
        }

        for(int i = 0; i < sol.size(); i++)
        {
            int literal = (sol[i] ? i + 1 : -(i + 1));
            // cout << literal << " ";
            result[i] = literal;
        }

        return true;
    }

    bool sat{true};
    static constexpr int MIN = std::numeric_limits<int>::min();
};

int main()
{
    static constexpr int MIN = std::numeric_limits<int>::min();

    // This code is here to increase the stack size to avoid stack overflow
    // in depth-first search.
    const rlim_t kStackSize = 64L * 1024L * 1024L; // min stack size = 64 Mb
    struct rlimit rl;
    int result;
    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }

    ios::sync_with_stdio(false);

    // fstream cin("./tests/01");

    int n, m;
    cin >> n >> m;
    TwoSatisfiability twoSat(n, m);
    for (int i = 0; i < m; ++i)
    {
        cin >> twoSat.clauses[i].firstVar >> twoSat.clauses[i].secondVar;
    }

    vector<int> satresult(n, MIN);
    if (twoSat.isSatisfiable(satresult))
    {
        cout << "SATISFIABLE" << endl;
        for (int i = 0; i < n; ++i)
        {
            cout << satresult[i] << " ";
        }
    }
    else
    {
        cout << "UNSATISFIABLE" << endl;
    }

    return 0;
}
