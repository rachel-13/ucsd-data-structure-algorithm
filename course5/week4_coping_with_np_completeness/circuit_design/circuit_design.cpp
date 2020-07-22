#include <sys/resource.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <fstream>
#include <stack>
#include <unordered_map>
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
    int index = -1;
    int sccID = -1;
    int lowLink = -1;
    bool onStack = false;
    vector<int> edges;
};

typedef vector<vector<int>> matrix;

struct TwoSatisfiability
{
public:
    int numVars;
    vector<Clause> clauses;

    unordered_map<long, Vertex> map;
    long adjListSize = 2 * numVars;
    int poppedItem = 0;
    int sccID = 0;
    int time = 0;
    vector<bool> sol;
    stack<long> stk;

    TwoSatisfiability(int n, int m) : numVars(n),
                                      clauses(m)
    {
    }

    void buildAdjList()
    {
        map = unordered_map<long, Vertex>(adjListSize);
        sol = vector<bool>(numVars);

        for (long i = 0; i < clauses.size(); i++)
        {
            Edge edge1 = Edge();
            edge1.from = -(clauses[i].firstVar);
            edge1.to = clauses[i].secondVar;
            

            Edge edge2 = Edge();
            edge2.from = -(clauses[i].secondVar);
            edge2.to = clauses[i].firstVar;

            map[edge1.from].edges.push_back(edge1.to);
            map[edge2.from].edges.push_back(edge2.to);
        }
    }

    void findComponentSets(int u)
    {
        // guided by https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm
        Vertex& u_info = map[u];
        u_info.index = u_info.lowLink = ++time;
        u_info.onStack = true;
        stk.push(u);

        for (auto v : u_info.edges)
        {   
            Vertex& nextV = map[v];
            if (nextV.index == -1)
            { //when v is not visited
                findComponentSets(v);
                u_info.lowLink = std::min(u_info.lowLink, nextV.lowLink);
            }
            else if (nextV.onStack) //when v is in the stack, update low for u
                u_info.lowLink = std::min(u_info.lowLink, nextV.index);
        }

        if (u_info.lowLink == u_info.index)
        {
            do
            {
                poppedItem = stk.top();
                stk.pop();

                if (sccID == map[-poppedItem].sccID)
                {
                    sat = false;
                    return;
                }

                // cout << poppedItem << " ";
                Vertex& v_info = map[poppedItem];
                v_info.sccID = sccID;
                v_info.onStack = false; //mark as item is popped
                sol[abs(poppedItem) - 1] = poppedItem < 0 ? true : false;
            } while (poppedItem != u);
            // cout << "\n";
            ++sccID;
        }
    }

    void searchAllScc()
    {
        for (int i = -numVars; i <= numVars; ++i) //initialize all elements
        {
            if (map[i].index == -1 && i != 0)
            {
                findComponentSets(i);
            }
        }
    }

    bool isSatisfiable(vector<long> &result)
    {
        buildAdjList();
        searchAllScc();

        if (!sat)
        {
            return false;
        }

        for (unsigned int i = 0; i < sol.size(); i++)
        {
            int literal = (sol[i] ? i + 1 : -(i + 1));
            result[i] = literal;
        }

        return true;
    }

    bool sat{true};
    static constexpr int MIN = std::numeric_limits<int>::min();
};

int main()
{

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

    vector<long> satresult(n);
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
