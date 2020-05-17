#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

using std::cin;
using std::cout;
using std::string;
using std::vector;

struct Query
{
    string type, s;
    size_t ind;
};

class QueryProcessor
{
    int bucket_count;
    // store all strings in one vector
    vector<string> elems;

    size_t hash_func(const string &s) const
    {
        static const size_t multiplier = 263;
        static const size_t prime = 1000000007;
        unsigned long long hash = 0;
        for (int i = static_cast<int>(s.size()) - 1; i >= 0; --i)
            hash = (hash * multiplier + s[i]) % prime;
        return hash % bucket_count;
    }

public:
    vector<vector<string>> table;

    explicit QueryProcessor(int bucket_count) : bucket_count(bucket_count) {}

    Query readQuery() const
    {
        Query query;
        cin >> query.type;
        if (query.type != "check")
            cin >> query.s;
        else
            cin >> query.ind;
        return query;
    }

    void writeSearchResult(bool was_found) const
    {
        std::cout << (was_found ? "yes\n" : "no\n");
    }

    vector<string> naive_test_results;
    void processQuery_naive(const Query &query)
    {
        if (query.type == "check")
        {
            // use reverse order, because we append strings to the end
            for (int i = static_cast<int>(elems.size()) - 1; i >= 0; --i)
                if (hash_func(elems[i]) == query.ind)
                {
                    std::cout << elems[i] << " ";
                    naive_test_results.push_back(elems[i]);
                }
            std::cout << "\n";
        }
        else
        {
            vector<string>::iterator it = std::find(elems.begin(), elems.end(), query.s);
            if (query.type == "find")
            {
                writeSearchResult(it != elems.end());
                if (it == elems.end())
                {
                    naive_test_results.push_back("no\n");
                }
                else
                {
                    naive_test_results.push_back("yes\n");
                }
            }
            else if (query.type == "add")
            {
                if (it == elems.end())
                    elems.push_back(query.s);
            }
            else if (query.type == "del")
            {
                if (it != elems.end())
                    elems.erase(it);
            }
        }
    }

    vector<string> test_results;
    void processQuery(const Query &query)
    {
        if (query.type == "check")
        {
            vector<string> content = table[query.ind];
            for (int i = 0; i < content.size(); i++)
            {
                cout << content[i] << " ";
                test_results.push_back(content[i]);
            }
            std::cout << "\n";
        }
        else
        {
            if (query.type == "add")
            {
                long long ind = hash_func(query.s);
                vector<string> content = table[ind];

                std::vector<string>::iterator itr = std::find(content.begin(), content.end(), query.s);
                if (itr != content.end())
                {
                    // if element found, do nothing
                }
                else
                {
                    content.insert(content.begin(), query.s);
                }
                table[ind] = content;
            }

            if (query.type == "find")
            {
                long long ind = hash_func(query.s);
                vector<string> content = table[ind];

                if (std::find(content.begin(), content.end(), query.s) != content.end())
                {
                    cout << "yes\n";
                    test_results.push_back("yes\n");
                }
                else
                {
                    cout << "no\n";
                    test_results.push_back("no\n");
                }
            }

            if (query.type == "del")
            {
                long long ind = hash_func(query.s);
                vector<string> content = table[ind];
                if (content.size() > 0)
                {
                    std::vector<string>::iterator itr = std::find(content.begin(), content.end(), query.s);
                    if (itr != content.end())
                    {
                        // element found, delete

                        int index = std::distance(content.begin(), itr);
                        content.erase(content.begin() + index);
                        table[ind] = content;
                    }
                }
            }
        }
    }

    void processQueries()
    {
        table = vector<vector<string>>(bucket_count);
        int n;
        cin >> n;
        for (int i = 0; i < n; ++i)
            processQuery(readQuery());
    }
};

string printstring(int n)
{
    char letters[52] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
                        'r', 's', 't', 'u', 'v', 'w', 'x',
                        'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
                        'R', 'S', 'T', 'U', 'V', 'W', 'X',
                        'Y', 'Z'};
    string ran = "";
    for (int i = 0; i < n; i++)
        ran = ran + letters[rand() % 52];
    return ran;
}

void test_solution()
{
    while (true)
    {
        int query_count = rand() % 50 + 5;
        int bucket_count = (query_count / 5);

        QueryProcessor proc(bucket_count);
        proc.table = vector<vector<string>>(bucket_count);

        // build dataset
        vector<string> randomString;
        vector<int> randomIndices;
        vector<string> types = {"add", "find", "del", "check"};
        for (int i = 0; i < query_count; i++)
        {
            string randomGeneratedString = printstring(15);
            randomString.push_back(randomGeneratedString);

            int ind = rand() % (bucket_count <= 0 ? bucket_count - 5 : bucket_count);
            randomIndices.push_back(ind);
        }

        // build queries
        vector<Query> queries;
        for (int i = 0; i < query_count; i++)
        {
            Query q = Query();
            q.type = types[rand() % 4];

            if (q.type == "check")
            {
                q.ind = randomIndices.back();
            }
            else
            {
                q.s = randomString.back();
            }
            queries.push_back(q);
        }

        // process queries
        for (int i = 0; i < queries.size(); i++)
        {
            proc.processQuery_naive(queries[i]);
            proc.processQuery(queries[i]);
        }

        // get results
        if (proc.naive_test_results == proc.test_results)
        {
            cout << "\n================================================\n";
            cout << "OK!"
                 << "\n";
        }
        else
        {
            cout << "\n================================================\n";
            cout << "Fail!"
                 << "\n";

            break;
        }
    }
}

int main()
{
    // test_solution();
    std::ios_base::sync_with_stdio(false);
    // std::ifstream file("./tests/03");
    // if (file.is_open())
    // {
    //     cin.rdbuf(file.rdbuf());
    // }

    int bucket_count;
    cin >> bucket_count;
    QueryProcessor proc(bucket_count);
    proc.processQueries();
    return 0;
}
